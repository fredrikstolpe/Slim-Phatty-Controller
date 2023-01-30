/*
  ==============================================================================

    SynthParameter.h
    Created: 29 Jan 2023 11:17:03am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiParameterType.h"

class SynthParameter
{
public:
    SynthParameter(juce::String id, int number);
    SynthParameter(juce::String id, int number, MidiParameterType type, int minValue, int maxValue, int defaultValue);
    juce::String id;
    int number;
    MidiParameterType type;
    int minValue;
    int maxValue = 127;
    int defaultValue = 0;
};