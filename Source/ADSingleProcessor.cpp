
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
#include "ADSingleProcessor.h"
#include "../WAW/waw.h"

//If the processor uses a custom editor, it needs its header to instantiate it
#include "ADSingleEditor.h"

ADSingleProcessor::ADSingleProcessor()
    : GenericProcessor("ADSingle"), _samprate(20000.0)
{
    parameters.add(Parameter("SampleRate", 1024.0, 50000.0, 20000.0, 0));
    parameters.add(Parameter("Scaling", 0.0, 1.0/0.0, 1.0, 1));
    setParameter(0, _samprate);
}

ADSingleProcessor::~ADSingleProcessor() {

}

/**
	If the processor uses a custom editor, this method must be present.
*/
AudioProcessorEditor* ADSingleProcessor::createEditor() {
	editor = new ADSingleEditor(this, false);

	return editor;
}

float ADSingleProcessor::getParameter(int parameterIndex) {
  float val;
  switch (parameterIndex) {
    case 0:
      val = _samprate;
      break;
    case 1:
      val = _scaleF;
      break;
    default:
      val = 0.0;
      break;
  }
  return val;
}

void ADSingleProcessor::setParameter(int parameterIndex, float newValue) {
  Parameter& p =  parameters.getReference(parameterIndex);
  p.setValue(newValue, 0);

  switch (parameterIndex) {
    case 0:
      _samprate = newValue;
      break;
    case 1:
      _scaleF = newValue;
      break;
    default:
      break;
  }
  // std::cout << float(p[0]) << std::endl;
}

void ADSingleProcessor::process(AudioSampleBuffer& buffer,MidiBuffer& events) {
	/**
	Generic structure for processing buffer data
	*/
	int nChannels = buffer.getNumChannels();
	for (int chan = 0; chan < nChannels; chan++)
	{
		int nSamples = getNumSamples(chan);
		/**
		Do something here.

		To obtain a read-only pointer to the n sample of a channel:
		float* samplePtr = buffer.getReadPointer(chan,n);

		To obtain a read-write pointer to the n sample of a channel:
		float* samplePtr = buffer.getWritePointer(chan,n);

		All the samples in a channel are consecutive, so this example is valid:
		float* samplePtr = buffer.getWritePointer(chan,0);
		for (i=0; i < nSamples; i++)
		{
			*(samplePtr+i) = (*samplePtr+i)+offset;
		}

		See also documentation and examples for buffer.copyFrom and buffer.addFrom to operate on entire channels at once.

		To add a TTL event generated on the n-th sample:
		addEvents(events, TTL, n);


		*/
	}
}
