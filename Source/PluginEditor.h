/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SlimPhattyControllerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SlimPhattyControllerAudioProcessorEditor (SlimPhattyControllerAudioProcessor&);
    ~SlimPhattyControllerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SlimPhattyControllerAudioProcessor& audioProcessor;

    juce::Label textLabel{ {}, "Midi output device" };
    juce::Font textFont{ 12.0f };
    juce::ComboBox midiOutMenu;

    void midiOutMenuChanaged();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlimPhattyControllerAudioProcessorEditor)
};
