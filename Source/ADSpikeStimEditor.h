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

#ifndef ADSPIKESTIMEDITOR_H_INCLUDED
#define ADSPIKESTIMEDITOR_H_INCLUDED

#include <EditorHeaders.h>

class ADSpikeStimEditor : public GenericEditor, public Label::Listener {
public:

  /** The class constructor, used to initialize any members. */
  ADSpikeStimEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);

  /** The class destructor, used to deallocate memory */
  ~ADSpikeStimEditor();

  /** Label listener callback, called when a label is changed. */
  void labelTextChanged(Label* label);

  /** This method executes whenever a custom button is pressed */
  void buttonEvent(Button* button);

  /** Called to inform the editor that acquisition is about to start*/
  void startAcquisition();

  /** Called to inform the editor that acquisition has just stopped*/
  void stopAcquisition();

  /** Called whenever there is a change in the signal chain or it refreshes.
  It's called after the processors' same named method.
  */
  //void updateSettings();

  bool isNodeEnabled();
  int channelNum();
  int sortNum();

private:
  //Always use JUCE RAII classes instead of pure pointers.
  ScopedPointer<Button> _enableButton;
  ScopedPointer<Label> _chNumLabel;
  ScopedPointer<Label> _chNumValue;
  ScopedPointer<Label> _sortNumLabel;
  ScopedPointer<Label> _sortNumValue;
  std::vector<Component *> _inputComponents;

  void initEditor();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSpikeStimEditor);
};
#endif
