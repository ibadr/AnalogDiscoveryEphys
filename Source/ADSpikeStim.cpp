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

ADSpikeStim::ADSpikeStim() : GenericProcessor("ADSpikeStim") {

}

ADSpikeStim::~ADSpikeStim() {

}

AudioProcessorEditor* ADSpikeStim::createEditor() {
  editor = new ADSpikeStimEditor(this, false);
	return editor;
}

void ADSpikeStim::process(AudioSampleBuffer& buffer, MidiBuffer& events) {

}
