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

#include "ADSpikeStim.h"
#include "ADSpikeStimEditor.h"
#include "../WAW/waw.h"

#include "../../Processors/Visualization/SpikeObject.h"

ADSpikeStim::ADSpikeStim() : GenericProcessor("ADSpikeStim"), _deviceOpen(false) {

}

ADSpikeStim::~ADSpikeStim() {
  if (_deviceOpen)
    WAW::instance().closeDevice(0);
}

AudioProcessorEditor* ADSpikeStim::createEditor() {
  editor = new ADSpikeStimEditor(this, false);
	return editor;
}

bool ADSpikeStim::isReady() {
  int count = WAW::instance().enumDevices(enumfilterAll);
  // std::cout << "Found " << count << std::endl;
  if (count > 0 && !_deviceOpen)
      _deviceOpen = WAW::instance().openDevice(0);
  if (count < 1)
    return false;
  if (_deviceOpen && count > 1) {
    BOOL ret;
    int hdwf = WAW::instance().hdwfDevice(0);
    ret=FDwfAnalogOutNodeEnableSet(hdwf, 0, AnalogOutNodeCarrier, true);
    // set square function
    ret = FDwfAnalogOutNodeFunctionSet(hdwf, 0, AnalogOutNodeCarrier, funcSquare);
    // 1 kHz
    ret=FDwfAnalogOutNodeFrequencySet(hdwf, 0, AnalogOutNodeCarrier, 1000.0);
    // 0.5 V amplitude, 1 V pk2pk
    ret=FDwfAnalogOutNodeAmplitudeSet(hdwf, 0, AnalogOutNodeCarrier, 0.5);
    // run for only 5 ms for each trigger
    ret = FDwfAnalogOutRunSet(hdwf, 0, 5*1e-3);
      return _deviceOpen && (ret>0) ? true:false;
  }
  return _deviceOpen;
}

void ADSpikeStim::process(AudioSampleBuffer& /*buffer*/, MidiBuffer& events) {
  checkForEvents(events); // automatically calls handleEvent
  // we're not interested in processing the continuous data in the 'buffer', only the spike events
}

void ADSpikeStim::handleEvent(int eventType, MidiMessage& event, int /*sampleNum*/) {
  if (eventType == SPIKE) {
    const uint8_t* dataptr = event.getRawData();
    int bufferSize = event.getRawDataSize();
    if (bufferSize > 0) {
      SpikeObject newSpike;
      bool isValid = unpackSpike(&newSpike, dataptr, bufferSize);
      if (isValid) {
        int hdwf = WAW::instance().hdwfDevice(0);
        if (newSpike.channel == 0 && newSpike.sortedId == 1) {
          FDwfAnalogOutConfigure(hdwf, 0, true); // run stim once
        }
      }
    }
  }
}
