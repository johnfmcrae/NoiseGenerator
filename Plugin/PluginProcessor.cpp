/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoiseGeneratorPluginAudioProcessor::NoiseGeneratorPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

NoiseGeneratorPluginAudioProcessor::~NoiseGeneratorPluginAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout NoiseGeneratorPluginAudioProcessor::createParameterLayout()
{
    //std::vector<std::unique_ptr<RangedAudioParameter>> params;
    AudioProcessorValueTreeState::ParameterLayout layout;

    // add the respective parameters
    layout.add(std::make_unique<AudioParameterFloat>(LEVEL_ID,  LEVEL_NAME, 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<AudioParameterBool> (WHITE_ID,  WHITE_NAME,  false));
    layout.add(std::make_unique<AudioParameterBool> (PINK_ID,   PINK_NAME,   false));
    layout.add(std::make_unique<AudioParameterBool> (STATE_ID,  STATE_NAME,  true));

    return layout;
}

//==============================================================================
const juce::String NoiseGeneratorPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseGeneratorPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGeneratorPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGeneratorPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoiseGeneratorPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseGeneratorPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoiseGeneratorPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseGeneratorPluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoiseGeneratorPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoiseGeneratorPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoiseGeneratorPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void NoiseGeneratorPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseGeneratorPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void NoiseGeneratorPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    // get level value
    auto levelSliderValue = treeState.getRawParameterValue(LEVEL_ID);

    //check noise type
    bool noiseIsWhite = treeState.getRawParameterValue(WHITE_ID)->load();
    bool noiseIsPink  = treeState.getRawParameterValue(PINK_ID)->load();

    // check if noise is on. Recall that we are checking an "OFF" button,
    // so if this button is ON, then we don't want any noise, i.e. check if it is false
    if (!treeState.getRawParameterValue(STATE_ID)->load())
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);

            // add noise to every sample in the buffer
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                if (noiseIsWhite)
                    channelData[sample] = random.nextFloat() * levelSliderValue->load() * 0.5; // white noise gen quieter than pink noise
                else if (noiseIsPink)
                    channelData[sample] = nP.generate() * levelSliderValue->load();
                else
                    channelData[sample] = buffer.getSample(channel, sample); // should never happen, but here as a catch
            }
        }

    }
    // if noise is off, use the slider as a level adjust
    else
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);

            // modify the volume by multiplying each sample value
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                channelData[sample] = buffer.getSample(channel, sample) * levelSliderValue->load();
            }
        }
    }
}

//==============================================================================
bool NoiseGeneratorPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoiseGeneratorPluginAudioProcessor::createEditor()
{
    return new NoiseGeneratorPluginAudioProcessorEditor (*this);
}

//==============================================================================
void NoiseGeneratorPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    std::unique_ptr<juce::XmlElement> xml(treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NoiseGeneratorPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    // error checking
    if (xmlState != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.state = juce::ValueTree::fromXml(*xmlState);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGeneratorPluginAudioProcessor();
}
