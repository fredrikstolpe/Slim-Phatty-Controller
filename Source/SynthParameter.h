/*
  ==============================================================================

    SynthParameter.h
    Created: 29 Jan 2023 11:17:03am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthParameter
{
public:
    SynthParameter(juce::String id, int ccNumber);
    juce::String id;
    int ccNumber;
    int minValue = 0;
    int maxValue = 128;
    int defaultValue = 0;
};