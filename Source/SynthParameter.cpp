/*
  ==============================================================================

    SynthParameter.cpp
    Created: 29 Jan 2023 11:17:03am
    Author:  fredrik.stolpe

  ==============================================================================
*/

#include "SynthParameter.h"

SynthParameter::SynthParameter(juce::String id, int ccNumber)
{
    this->id = id;
    this->ccNumber = ccNumber;
}