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

#ifndef ADSUPPLY_H_INCLUDED
#define ADSUPPLY_H_INCLUDED


#ifdef _WIN32
#include <Windows.h>
#endif

#include <ProcessorHeaders.h>

class ADSupply : public GenericProcessor {

public:
  ADSupply();
  ~ADSupply();

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

  void process(AudioSampleBuffer& buffer, MidiBuffer& events);

private:

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSupply);

};

#endif
