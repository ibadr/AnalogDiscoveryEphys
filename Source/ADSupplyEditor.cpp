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

#include "ADSupplyEditor.h"

#include "../WAW/waw.h"

ADSupplyEditor::ADSupplyEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors = false)
  : GenericEditor(parentNode, useDefaultParameterEditors) {
    desiredWidth = 150;

    initEditor();
    int count = WAW::instance().enumDevices(enumfilterAll);
    _deviceOpen = WAW::instance().openDevice(0);
}

ADSupplyEditor::~ADSupplyEditor() {
  if (_deviceOpen)
    WAW::instance().closeDevice(0);
}

void ADSupplyEditor::initEditor() {
  _enableButton = new ToggleButton("ENABLE");
  _enableButton->setBounds(10, 31, 60, 20); //Set position and size (X, Y, XSize, YSize)
  _enableButton->setTooltip("Enable power supply output.");
  _enableButton->addListener(this);
  addAndMakeVisible(_enableButton);

  _posVoltLabel = new Label("positive volt label", "+V:");
  _posVoltLabel->setBounds(10, 60, 80, 20);
  _posVoltLabel->setFont(Font("Small Text", 11, Font::plain));
  _posVoltLabel->setColour(Label::textColourId, Colours::darkgrey);
  addAndMakeVisible(_posVoltLabel);

  _posVoltValue = new Label("positive volt value", String(5.0));
  _posVoltValue->setBounds(75, 60, 35, 20);
  _posVoltValue->setFont(Font("Default", 11, Font::plain));
  _posVoltValue->setColour(Label::textColourId, Colours::white);
  _posVoltValue->setColour(Label::backgroundColourId, Colours::grey);
  _posVoltValue->setEditable(true);
  _posVoltValue->addListener(this);
  _posVoltValue->setTooltip("Set positive supply voltage (only Analog Discovery 2)");
  addAndMakeVisible(_posVoltValue);
  _inputComponents.push_back(_posVoltValue);

  _negVoltLabel = new Label("negative volt label", "-V:");
  _negVoltLabel->setBounds(10, 90, 80, 20);
  _negVoltLabel->setFont(Font("Small Text", 11, Font::plain));
  _negVoltLabel->setColour(Label::textColourId, Colours::darkgrey);
  addAndMakeVisible(_negVoltLabel);

  _negVoltValue = new Label("sort number value", String(-5.0));
  _negVoltValue->setBounds(75, 90, 35, 20);
  _negVoltValue->setFont(Font("Default", 11, Font::plain));
  _negVoltValue->setColour(Label::textColourId, Colours::white);
  _negVoltValue->setColour(Label::backgroundColourId, Colours::grey);
  _negVoltValue->setEditable(true);
  _negVoltValue->addListener(this);
  _negVoltValue->setTooltip("Set negative supply voltage (only Analog Discovery 2)");
  addAndMakeVisible(_negVoltValue);
  _inputComponents.push_back(_negVoltValue);
}

void ADSupplyEditor::labelTextChanged(Label* label) {

}

void ADSupplyEditor::buttonEvent(Button* button) {
  if (button == _enableButton) {
    if (!_deviceOpen)
      _deviceOpen = WAW::instance().openDevice(0);
    if (_deviceOpen) {
      int hdwf = WAW::instance().hdwfDevice(0);
      if (_enableButton->getToggleState()) {
        FDwfAnalogIOChannelNodeSet(hdwf, 0, 0, 1);
        FDwfAnalogIOChannelNodeSet(hdwf, 0, 1, _posVoltValue->getText().getFloatValue());
        FDwfAnalogIOChannelNodeSet(hdwf, 1, 0, 1);
        FDwfAnalogIOChannelNodeSet(hdwf, 1, 1, _negVoltValue->getText().getFloatValue());
        FDwfAnalogIOEnableSet(hdwf, 1);
      } else {
        FDwfAnalogIOEnableSet(hdwf, 0);
      }
    }
  }
}

void ADSupplyEditor::startAcquisition() {
  for (std::vector<Component *>::iterator itr = _inputComponents.begin();
    itr != _inputComponents.end(); ++itr) {
      (*itr)->setEnabled(false);
  }
}

void ADSupplyEditor::stopAcquisition() {
  for (std::vector<Component *>::iterator itr = _inputComponents.begin();
    itr != _inputComponents.end(); ++itr) {
      (*itr)->setEnabled(true);
  }
}
