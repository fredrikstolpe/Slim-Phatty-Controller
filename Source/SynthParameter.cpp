/*
  ==============================================================================

    SynthParameter.cpp
    Created: 29 Jan 2023 11:17:03am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#include "SynthParameter.h"

SynthParameter::SynthParameter(juce::String id, int number)
{
    this->id = id;
    this->number = number;
    this->type = CC;
    this->minValue = 0;
    this->maxValue = 127;
    this->defaultValue = 0;
}

SynthParameter::SynthParameter(juce::String id, int number, MidiParameterType type, int minValue, int maxValue, int defaultValue)
{
    this->id = id;
    this->number = number;
    this->type = type;
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->defaultValue = defaultValue;
}
