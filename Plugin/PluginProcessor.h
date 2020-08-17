/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinkNoise.h"
// defines for consistent IDs and names
#define LEVEL_ID    "level"
#define LEVEL_NAME  "Level"
#define WHITE_ID    "white"
#define WHITE_NAME  "White Noise"
#define PINK_ID     "pink"
#define PINK_NAME   "Pink Noise"
#define STATE_ID    "state"
#define STATE_NAME  "On/Off"

//==============================================================================
/**
*/
class NoiseGeneratorPluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    NoiseGeneratorPluginAudioProcessor();
    ~NoiseGeneratorPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // control variables
    double levelValue;
    bool whiteSw;
    bool pinkSw;
    bool offSw;

    //juce::AudioProcessorValueTreeState treeState;
    juce::AudioProcessorValueTreeState treeState;

private:
    // noise classses
    juce::Random random;
    PinkNoise nP;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoiseGeneratorPluginAudioProcessor)
};
