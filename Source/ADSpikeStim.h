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

#ifndef ADSPIKESTIM_H_INCLUDED
#define ADSPIKESTIM_H_INCLUDED


#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>

class ADSpikeStim : public GenericProcessor {

public:
  ADSpikeStim();
  ~ADSpikeStim();

  bool isSource() {
    return false;
  }
  bool isSink() {
    return true;
  }
  bool hasEditor() const {
    return true;
  }

  /** If the processor has a custom editor, this method must be defined to instantiate it. */
  AudioProcessorEditor* createEditor();

  /** Optional method that informs the GUI if the processor is ready to function. If false acquisition cannot start. Defaults to true */
  //bool isReady();

  void process(AudioSampleBuffer& buffer, MidiBuffer& events);

  /** The method that standard controls on the editor will call.

  It is recommended that any variables used by the "process" function
  are modified only through this method while data acquisition is active. */
  // void setParameter(int parameterIndex, float newValue);

  /** Optional method called every time the signal chain is refreshed or changed in any way.

  Allows the processor to handle variations in the channel configuration or any other parameter
  passed down the signal chain. The processor can also modify here the settings structure, which contains
  information regarding the input and output channels as well as other signal related parameters. Said
  structure shouldn't be manipulated outside of this method.
  */
  //void updateSettings();

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSpikeStim);

};

#endif
