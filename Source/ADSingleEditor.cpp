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

#include "ADSingleEditor.h"
#include "ADSingleThread.h"
#include "../WAW/waw.h"

ADSingleEditor::ADSingleEditor(GenericProcessor* parentNode,ADSingleThread* ads, bool useDefaultParameterEditors = false)
	: GenericEditor(parentNode, useDefaultParameterEditors)
{
	desiredWidth = 230;
	_deviceOpen = false;

	initEditor();
	buttonEvent(_refreshButton); // emulate refresh button press after construction
}

ADSingleEditor::~ADSingleEditor() {
		if (_deviceOpen)
			WAW::instance().closeDevice(0);
}

void ADSingleEditor::initEditor() {

	// ADSingleThread * node = static_cast<ADSingleThread*>(this->getProcessor());

	_refreshButton = new UtilityButton("REFRESH", Font(5, Font::plain));
	_refreshButton->setBounds(150, 31, 60, 20); //Set position and size (X, Y, XSize, YSize)
	_refreshButton->setTooltip("Scan for and open first device.");
	_refreshButton->addListener(this);
	addAndMakeVisible(_refreshButton);

	_deviceLabel = new Label("device label", "No devices!");
	_deviceLabel->setBounds(10, 31, 150, 20);
	_deviceLabel->setFont(Font("Small Text", 11, Font::plain));
	_deviceLabel->setColour(Label::textColourId, Colours::darkgrey);
	addAndMakeVisible(_deviceLabel);

	_sampleRateLabel = new Label("sample rate label", "Sample rate:");
	_sampleRateLabel->setBounds(10, 65, 80, 20);
	_sampleRateLabel->setFont(Font("Small Text", 11, Font::plain));
	_sampleRateLabel->setColour(Label::textColourId, Colours::darkgrey);
	addAndMakeVisible(_sampleRateLabel);

	_sampleRateValue = new Label("sample rate value", String(25000/*node->getParameter(0)*/));
	_sampleRateValue->setBounds(100, 65, 50, 20);
	_sampleRateValue->setFont(Font("Default", 11, Font::plain));
	_sampleRateValue->setColour(Label::textColourId, Colours::white);
	_sampleRateValue->setColour(Label::backgroundColourId, Colours::grey);
	_sampleRateValue->setEditable(true);
	_sampleRateValue->addListener(this);
	_sampleRateValue->setTooltip("Set the sample rate for the selected channels");
	addAndMakeVisible(_sampleRateValue);

	_scaleFLabel = new Label("scaling factor label", "Scaling:");
	_scaleFLabel->setBounds(10, 90, 80, 20);
	_scaleFLabel->setFont(Font("Small Text", 11, Font::plain));
	_scaleFLabel->setColour(Label::textColourId, Colours::darkgrey);
	addAndMakeVisible(_scaleFLabel);

	_scaleFValue = new Label("scaling factor value", String(1.0));
	_scaleFValue->setBounds(100, 90, 50, 20);
	_scaleFValue->setFont(Font("Default", 11, Font::plain));
	_scaleFValue->setColour(Label::textColourId, Colours::white);
	_scaleFValue->setColour(Label::backgroundColourId, Colours::grey);
	_scaleFValue->setEditable(true);
	_scaleFValue->addListener(this);
	_scaleFValue->setTooltip("Set the scaling factor for all channels");
	addAndMakeVisible(_scaleFValue);
}

void ADSingleEditor::labelTextChanged(Label* label) {
	// ADSingleThread * node = static_cast<ADSingleThread*>(this->getProcessor());
	// if (label == _scaleFValue)
	// {
	// 	_node->setDefaultBitVolts(label->getText().getFloatValue());
	// }
	// else
	if (label == _sampleRateValue)
	{
		float v = label->getText().getFloatValue();
		if (v < 1024.0) {
			CoreServices::sendStatusMessage("Please set the sample rate to at least 1024.0 Hz.");
			label->setText(String(25000/*node->getDefaultSampleRate()*/), NotificationType::dontSendNotification);
			return;
		}
		// _node->setDefaultSampleRate(v);
	}
	// else if (label == _setChanNumValue)
	// {
	// 	int v = label->getText().getIntValue();
	// 	_node->setNumChannels(v);
	// }
	return;
}

void ADSingleEditor::buttonEvent(Button* button) {
	if (button == _refreshButton) {
		int count = WAW::instance().enumDevices(enumfilterAll);
		// std::cout << "Found " << count << std::endl;
		if (count > 0 && !_deviceOpen)
				_deviceOpen = WAW::instance().openDevice(0);
		if (count < 1) {
			_deviceLabel->setText("No devices!",NotificationType::dontSendNotification);
			return;
		}
		if (_deviceOpen) {
			char deviceName[32];
			FDwfEnumDeviceName(0, deviceName);
			_deviceLabel->setText(String(deviceName),NotificationType::dontSendNotification);
		}
	}

}
