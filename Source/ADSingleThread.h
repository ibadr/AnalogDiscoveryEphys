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

#ifndef ADSINGLE_H_INCLUDED
#define ADSINGLE_H_INCLUDED

#ifdef _WIN32
#include <Windows.h>
#endif

#include <DataThreadHeaders.h>

/**

  This processor provides a source node using Digilent's Waveforms SDK

  @see GenericProcessor

*/

class ADSingleThread : public DataThread
{
public:
  ADSingleThread(SourceNode* sn);
  ~ADSingleThread();

  bool foundInputSource();
  int getNumHeadstageOutputs();
  bool startAcquisition();
  bool stopAcquisition();
  int getNumChannels();
  float getSampleRate();
  float getBitVolts(Channel* chan);

	GenericEditor* createEditor(SourceNode* sn);

private:
    float _samprate;
    float _scaleF;

    float thisSample[16];

    bool updateBuffer();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSingleThread);

};

#endif  // ADSINGLE_H_INCLUDED
