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

#include "ADSpikeStimEditor.h"

ADSpikeStimEditor::ADSpikeStimEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors = false)
  : GenericEditor(parentNode, useDefaultParameterEditors) {
    desiredWidth = 150;

    initEditor();
}

ADSpikeStimEditor::~ADSpikeStimEditor() {

}

void ADSpikeStimEditor::initEditor() {
  _enableButton = new ToggleButton("ENABLE");
  _enableButton->setBounds(10, 31, 60, 20); //Set position and size (X, Y, XSize, YSize)
  _enableButton->setTooltip("Enable stimulator output.");
  _enableButton->addListener(this);
  addAndMakeVisible(_enableButton);

  _chNumLabel = new Label("channel number label", "Channel:");
  _chNumLabel->setBounds(10, 60, 80, 20);
  _chNumLabel->setFont(Font("Small Text", 11, Font::plain));
  _chNumLabel->setColour(Label::textColourId, Colours::darkgrey);
  addAndMakeVisible(_chNumLabel);

  _chNumValue = new Label("channel number value", String(1.0));
  _chNumValue->setBounds(75, 60, 35, 20);
  _chNumValue->setFont(Font("Default", 11, Font::plain));
  _chNumValue->setColour(Label::textColourId, Colours::white);
  _chNumValue->setColour(Label::backgroundColourId, Colours::grey);
  _chNumValue->setEditable(true);
  _chNumValue->addListener(this);
  _chNumValue->setTooltip("Set the channel number on which to listen to spike events");
  addAndMakeVisible(_chNumValue);
  _inputComponents.push_back(_chNumValue);

  _sortNumLabel = new Label("sort number label", "Sort:");
  _sortNumLabel->setBounds(10, 90, 80, 20);
  _sortNumLabel->setFont(Font("Small Text", 11, Font::plain));
  _sortNumLabel->setColour(Label::textColourId, Colours::darkgrey);
  addAndMakeVisible(_sortNumLabel);

  _sortNumValue = new Label("sort number value", String(1.0));
  _sortNumValue->setBounds(75, 90, 35, 20);
  _sortNumValue->setFont(Font("Default", 11, Font::plain));
  _sortNumValue->setColour(Label::textColourId, Colours::white);
  _sortNumValue->setColour(Label::backgroundColourId, Colours::grey);
  _sortNumValue->setEditable(true);
  _sortNumValue->addListener(this);
  _sortNumValue->setTooltip("Set the sort number that triggers the stimulator");
  addAndMakeVisible(_sortNumValue);
  _inputComponents.push_back(_sortNumValue);
}

void ADSpikeStimEditor::labelTextChanged(Label* label) {

}

void ADSpikeStimEditor::buttonEvent(Button* button) {

}

void ADSpikeStimEditor::startAcquisition() {
  for (std::vector<Component *>::iterator itr = _inputComponents.begin();
    itr != _inputComponents.end(); ++itr) {
      (*itr)->setEnabled(false);
  }
}

void ADSpikeStimEditor::stopAcquisition() {
  for (std::vector<Component *>::iterator itr = _inputComponents.begin();
    itr != _inputComponents.end(); ++itr) {
      (*itr)->setEnabled(true);
  }
}
