
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

ADSingleThread::ADSingleThread(SourceNode* sn)
    : DataThread(sn), _samprate(20000.0) {
      dataBuffer = new DataBuffer(16, 1600*3);
      eventCode = 0;
}

ADSingleThread::~ADSingleThread() {

}

GenericEditor* ADSingleThread::createEditor(SourceNode* sn) {
	return new ADSingleEditor(sn, this, true);
}

bool ADSingleThread::foundInputSource() {
    return true;
}

int ADSingleThread::getNumChannels() {
    return 16;
}

float ADSingleThread::getSampleRate() {
    return 28000.0f;
}

float ADSingleThread::getBitVolts(Channel* chan) {
    return 0.0305f;
}

int ADSingleThread::getNumHeadstageOutputs() {
  return 3;
}

bool ADSingleThread::startAcquisition() {
    startThread();
    return true;
}

bool ADSingleThread::stopAcquisition() {
    if (isThreadRunning())
    {
        signalThreadShouldExit();
    }

    return true;
}

bool ADSingleThread::updateBuffer() {
  int chan = 0;

  for (int n = 0; n < 1600; n++)
  {
    thisSample[chan] = n;

    if (chan == 15)
    {
        timestamp++;
        dataBuffer->addToBuffer(thisSample, &timestamp, &eventCode, 1);
        chan = 0;
    }
    else
    {
        chan++;
    }
  }
  return true;
}
