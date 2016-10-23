
/*
    ------------------------------------------------------------------

    Copyright (C) 2016 Islam Badreldin

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include <stdio.h>
#include "ADSingleThread.h"
#include "../WAW/waw.h"

//If the processor uses a custom editor, it needs its header to instantiate it
#include "ADSingleEditor.h"

const int MAX_CH_COUNT = 4; // TODO what is maximum channel count? 4?

ADSingleThread::ADSingleThread(SourceNode* sn)
    : DataThread(sn), _chCount(1), _thisSample(NULL), _theSourceNode(sn) {
  dataBuffer = new DataBuffer(MAX_CH_COUNT, 1600*3);
  eventCode = 0;
}

ADSingleThread::~ADSingleThread() {

}

GenericEditor* ADSingleThread::createEditor(SourceNode* sn) {
	return new ADSingleEditor(sn, this, true);
}

bool ADSingleThread::foundInputSource() {
  HDWF hdwf = WAW::instance().hdwfDevice(0);
  if(hdwf > 0) {
    BOOL succ; int chCount;
    succ = FDwfAnalogInChannelCount(hdwf, &chCount);
    if (succ<1) return false;
    _chCount = chCount;

    // For testing purposes only, enable a sine out on W1
    // int ch = 0;
    // FDwfAnalogOutNodeEnableSet(hdwf,ch,AnalogOutNodeCarrier,1);
    // FDwfAnalogOutNodeFunctionSet(hdwf,ch,AnalogOutNodeCarrier,funcSine);
    // FDwfAnalogOutNodeFrequencySet(hdwf,ch,AnalogOutNodeCarrier,7.0);
    // FDwfAnalogOutNodeAmplitudeSet(hdwf,ch,AnalogOutNodeCarrier,1.0);
    // FDwfAnalogOutNodeOffsetSet(hdwf,ch,AnalogOutNodeCarrier,0.0);
    // FDwfAnalogOutConfigure(hdwf,ch,1);

    return true;
  }
  return false;
}

int ADSingleThread::getNumChannels() {
  return _chCount;
}

float ADSingleThread::getSampleRate() {
  ADSingleEditor * edt = (ADSingleEditor *) _theSourceNode->editor.get();
  return edt->sampleRate();
}

float ADSingleThread::getBitVolts(Channel* chan) {
    return 1.0f;
}

int ADSingleThread::getNumHeadstageOutputs() {
  return _chCount;
}

bool ADSingleThread::startAcquisition() {
  ADSingleEditor * edt = (ADSingleEditor *) _theSourceNode->editor.get();
  HDWF hdwf = WAW::instance().hdwfDevice(0);
  bool isSuccess = false;
  BOOL succ;
  for(int tmp=0; tmp<1; ++tmp) { // so that 'break' works
    if(hdwf > 0) {
      succ = FDwfAnalogInFrequencySet(hdwf,edt->sampleRate());
      isSuccess = succ > 0; if(!isSuccess) break;
      succ = FDwfAnalogInAcquisitionModeSet(hdwf,acqmodeRecord);
      isSuccess = succ > 0;  if(!isSuccess) break;
      int chCount;
      succ = FDwfAnalogInChannelCount(hdwf, &chCount);
      isSuccess = succ > 0;  if(!isSuccess) break;
      for(int ch = 0; ch < chCount; ++ch) {
        succ = FDwfAnalogInChannelEnableSet(hdwf,ch,1);
        isSuccess = succ > 0;  if(!isSuccess) break;
        succ = FDwfAnalogInChannelFilterSet(hdwf,ch,filterAverage);
        isSuccess = succ > 0;  if(!isSuccess) break;
      }
      succ = FDwfAnalogInRecordLengthSet(hdwf,0.0); // record indefinitely
      isSuccess = succ > 0; if(!isSuccess) break;
      succ = FDwfAnalogInConfigure(hdwf,0,1); // start acquistion
      isSuccess = succ > 0; if(!isSuccess) break;
      _chCount = chCount;

      delete [] _thisSample;
      _thisSample = new float[_chCount];

      startThread();
      return isSuccess;
    }
  }
  return isSuccess;
}

bool ADSingleThread::stopAcquisition() {
  HDWF hdwf = WAW::instance().hdwfDevice(0);
  FDwfAnalogInConfigure(hdwf,0,1);
  if (isThreadRunning()) {
      signalThreadShouldExit();
  }
  return true;
}

bool ADSingleThread::updateBuffer() {
  ADSingleEditor * edt = (ADSingleEditor *) _theSourceNode->editor.get();
  float scale = edt->scaleF();

  HDWF hdwf = WAW::instance().hdwfDevice(0);
  DwfState status; BOOL succ;
  succ = FDwfAnalogInStatus(hdwf,1.0,&status); // read data
  if(succ<1) return false;

  if (status == DwfStateConfig || status == DwfStatePrefill || status == DwfStateArmed)
    return false;
  int cAvailable,cLost,cCorrupted;
  succ = FDwfAnalogInStatusRecord(hdwf,&cAvailable,&cLost,&cCorrupted);
  // std::cout << cAvailable << " " << cLost << " " << cCorrupted << std::endl;
  if (succ<1) return false;
  if (cAvailable==0) return false;

  double chVoltDataBuf[MAX_CH_COUNT][10000];
  for(int ch = 0; ch < _chCount; ++ch) {
    succ = FDwfAnalogInStatusData(hdwf,ch,chVoltDataBuf[ch],cAvailable);
    if (succ<1) return false;
  }
  for (int n = 0; n < cAvailable; ++n) {
    for(int ch = 0; ch < _chCount; ++ch) {
      _thisSample[ch] = chVoltDataBuf[ch][n]*scale;
    }
    timestamp++;
    dataBuffer->addToBuffer(_thisSample, &timestamp, &eventCode, 1);
  }
  return true;
}
