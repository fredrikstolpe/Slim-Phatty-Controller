/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SlimPhattyControllerAudioProcessorEditor::SlimPhattyControllerAudioProcessorEditor (SlimPhattyControllerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    auto availableMidiOutDevices = juce::MidiOutput::getAvailableDevices();

    addAndMakeVisible(textLabel);
    textLabel.setFont(textFont);

    // add items to the combo-box
    addAndMakeVisible(midiOutMenu);

    int i = 0;
    for (juce::MidiDeviceInfo& device : availableMidiOutDevices)
    {
        midiOutMenu.addItem(device.name, i);
        i++;
    }

    midiOutMenu.onChange = [this] { midiOutMenuChanaged(); };
    midiOutMenu.setSelectedId(0);
    midiOutMenuChanaged();

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

void SlimPhattyControllerAudioProcessorEditor::midiOutMenuChanaged()
{
    int id = midiOutMenu.getSelectedId();
    auto availableMidiOutDevices = juce::MidiOutput::getAvailableDevices();
    int i = 0;
    for (juce::MidiDeviceInfo& device : availableMidiOutDevices)
    {
        if (i == id) {
            audioProcessor.changeMidiDevice(device.identifier);
        }
        i++;
    }
}

SlimPhattyControllerAudioProcessorEditor::~SlimPhattyControllerAudioProcessorEditor()
{
}

//==============================================================================
void SlimPhattyControllerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SlimPhattyControllerAudioProcessorEditor::resized()
{
    textLabel.setBounds(10, 10, getWidth() - 20, 20);
    midiOutMenu.setBounds(10, 40, getWidth() - 20, 20);
}
