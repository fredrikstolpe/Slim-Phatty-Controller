/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SlimPhattyControllerAudioProcessor::SlimPhattyControllerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    treeState(*this, nullptr)
#endif
{
    parameters = createParameters();

    std::map<juce::String, SynthParameter*>::iterator iterator;

    for (iterator = parameters.begin(); iterator != parameters.end(); iterator++)
    {
        treeState.createAndAddParameter(std::make_unique<juce::AudioParameterInt>(iterator->first, iterator->second->id, iterator->second->minValue, iterator->second->maxValue, iterator->second->defaultValue));
        treeState.addParameterListener(iterator->first, this);
    }

    auto availableMidiOutDevices = juce::MidiOutput::getAvailableDevices();

    int i = 0;
    for (juce::MidiDeviceInfo& device : availableMidiOutDevices)
    {
        if (device.name == "Slim Phatty")
        {
            changeMidiDevice(device.identifier);
        }
    }
}

SlimPhattyControllerAudioProcessor::~SlimPhattyControllerAudioProcessor()
{
}

//==============================================================================
const juce::String SlimPhattyControllerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SlimPhattyControllerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SlimPhattyControllerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SlimPhattyControllerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SlimPhattyControllerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SlimPhattyControllerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SlimPhattyControllerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SlimPhattyControllerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SlimPhattyControllerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SlimPhattyControllerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SlimPhattyControllerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SlimPhattyControllerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SlimPhattyControllerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SlimPhattyControllerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

void SlimPhattyControllerAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    int ccValue = static_cast<int>(newValue);

    if (midiOutput)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(1, parameters[parameterID]->ccNumber, ccValue));
    }
}

void SlimPhattyControllerAudioProcessor::changeMidiDevice(juce::String& identifier) {
    midiOutput = juce::MidiOutput::openDevice(identifier);
}

std::map<juce::String, SynthParameter*> SlimPhattyControllerAudioProcessor::createParameters() {
    std::map<juce::String, SynthParameter*> dict;

    dict.insert(std::make_pair("GLIDE_ON_OFF", new SynthParameter("GLIDE_ON_OFF", 65)));
    dict.insert(std::make_pair("OSC1_WAVE", new SynthParameter("OSC1_WAVE", 9)));
    dict.insert(std::make_pair("OSC1_OCTAVE", new SynthParameter("OSC1_OCTAVE", 74)));
    dict.insert(std::make_pair("SYNC_ON_OFF", new SynthParameter("SYNC_ON_OFF", 77)));
    dict.insert(std::make_pair("OSC2_WAVE", new SynthParameter("OSC2_WAVE", 11)));
    dict.insert(std::make_pair("OSC2_OCTAVE", new SynthParameter("OSC2_WAVE", 75)));
    dict.insert(std::make_pair("GLIDE_RATE", new SynthParameter("GLIDE_RATE", 5)));
    dict.insert(std::make_pair("OSC2_TUNE", new SynthParameter("OSC2_TUNE", 10)));
    dict.insert(std::make_pair("MOD_SOURCE5", new SynthParameter("MOD_SOURCE5", 104)));
    dict.insert(std::make_pair("MOD_DEST2", new SynthParameter("MOD_DEST2", 106)));
    dict.insert(std::make_pair("OSC1_LEVEL", new SynthParameter("OSC1_LEVEL", 15)));
    dict.insert(std::make_pair("OSC2_LEVEL", new SynthParameter("OSC2_LEVEL", 16)));
    dict.insert(std::make_pair("OVERDRIVE", new SynthParameter("OVERDRIVE", 18)));
    dict.insert(std::make_pair("LPF_POLES", new SynthParameter("LPF_POLES", 109)));
    dict.insert(std::make_pair("MOD_AMOUNT", new SynthParameter("MOD_AMOUNT", 6)));
    dict.insert(std::make_pair("MOD_DESTINATION", new SynthParameter("MOD_DESTINATION", 69)));
    dict.insert(std::make_pair("LPF_CUTOFF", new SynthParameter("LPF_CUTOFF", 19)));
    dict.insert(std::make_pair("LPF_RESONANCE", new SynthParameter("LPF_RESONANCE", 21)));
    dict.insert(std::make_pair("LPF_KB_AMOUNT", new SynthParameter("LPF_KB_AMOUNT", 22)));
    dict.insert(std::make_pair("LPF_ENV2_AMOUNT", new SynthParameter("LPF_ENV2_AMOUNT", 27)));
    dict.insert(std::make_pair("MOD_SOURCE", new SynthParameter("MOD_SOURCE", 68)));
    dict.insert(std::make_pair("MOD_RATE", new SynthParameter("MOD_RATE", 3)));
    dict.insert(std::make_pair("FILTER_ENV_DECAY", new SynthParameter("FILTER_ENV_DECAY", 24)));
    dict.insert(std::make_pair("FILTER_ENV_SUSTAIN", new SynthParameter("FILTER_ENV_SUSTAIN", 25)));
    dict.insert(std::make_pair("FILTER_ENV_RELEASE", new SynthParameter("FILTER_ENV_RELEASE", 26)));
    dict.insert(std::make_pair("FILTER_ENV_ATTACK", new SynthParameter("FILTER_ENV_ATTACK", 23)));
    dict.insert(std::make_pair("AMP_ENV_ATTACK", new SynthParameter("AMP_ENV_ATTACK", 28)));
    dict.insert(std::make_pair("AMP_ENV_DECAY", new SynthParameter("AMP_ENV_DECAY", 29)));
    dict.insert(std::make_pair("AMP_ENV_SUSTAIN", new SynthParameter("AMP_ENV_SUSTAIN", 30)));
    dict.insert(std::make_pair("AMP_ENV_RELEASE", new SynthParameter("AMP_ENV_RELEASE", 31)));

    return dict;
}

//==============================================================================
bool SlimPhattyControllerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SlimPhattyControllerAudioProcessor::createEditor()
{
    return new SlimPhattyControllerAudioProcessorEditor (*this);
}

//==============================================================================
void SlimPhattyControllerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SlimPhattyControllerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SlimPhattyControllerAudioProcessor();
}
