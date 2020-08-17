/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Apple II font from http://www.kreativekorp.com/software/fonts/apple2.shtml
    // this line is important to ensure that the custom font is used
    LookAndFeel::setDefaultLookAndFeel(&LaF);

    // LABELS
    titleLabel.setText("Noise Generator", dontSendNotification);
    titleLabel.setJustificationType(Justification::centred);
    titleLabel.setFont(Font(18.0f, Font::bold));
    titleLabel.setColour(Label::textColourId, Colours::green);

    levelLabel.setText("Level", dontSendNotification);
    levelLabel.setJustificationType(Justification::centred);
    levelLabel.setColour(Label::textColourId, Colours::green);
    levelLabel.setFont(Font(12.0f, Font::bold));

    addAndMakeVisible(&titleLabel);
    addAndMakeVisible(&levelLabel);

    // BUTTONS
    setupOldSchoolAndFeelColours(LaF);

    wButton.setButtonText("White");
    pButton.setButtonText("Pink");
    offButton.setButtonText("Off");
    stereoB.setButtonText("Stereo");

    wButton.onClick = [this] { updateToggleState(&wButton, "White"); };
    pButton.onClick = [this] { updateToggleState(&pButton, "Pink");  };
    offButton.onClick = [this] { updateToggleState(&offButton, "Off");   };

    // add to radio ID group so that they operate in mutually exclusive mode
    wButton.setRadioGroupId(NoiseButtons);
    pButton.setRadioGroupId(NoiseButtons);
    offButton.setRadioGroupId(NoiseButtons);

    // set formatting
    wButton.setLookAndFeel(&LaF);
    pButton.setLookAndFeel(&LaF);
    offButton.setLookAndFeel(&LaF);
    stereoB.setLookAndFeel(&LaF);

    // Set edges for the noise selection row
    wButton.setConnectedEdges(2);
    pButton.setConnectedEdges(3);
    offButton.setConnectedEdges(1);

    // set the noise buttons to not be toggle switches
    wButton.setClickingTogglesState(true);
    pButton.setClickingTogglesState(true);
    offButton.setClickingTogglesState(true);
    // set the stereo button to be a toggle switch
    stereoB.setClickingTogglesState(true);

    addAndMakeVisible(&wButton);
    addAndMakeVisible(&pButton);
    addAndMakeVisible(&offButton);
    addAndMakeVisible(&stereoB);

    // SLIDER
    levelSlider.setSliderStyle(Slider::LinearBar);
    levelSlider.setRange(0.0, 0.5);
    levelSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    levelSlider.setLookAndFeel(&LaF);
    levelSlider.setColour(Slider::backgroundColourId, Colours::black);
    addAndMakeVisible(&levelSlider);

    // Make sure you set the size of the component after
    // you add any child components.
    setSize(350, 200);
    setAudioChannels(0, 2); // no inputs, two outputs
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlockExpected << "\n";
    message << " sampleRate = " << sampleRate;
    Logger::getCurrentLogger()->writeToLog(message);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // read level value from the slider
    auto level = (float)levelSlider.getValue();

    // check noise type
    // white noise
    if (wButton.getToggleState())
    {
        // check if stereo
        if (stereoB.getToggleState())
        {
            // generate stereo white noise, meaning separate noise values in each channel
            for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    *buffer++ = random.nextFloat() * level;
            }
        }
        else
        {
            // channel 0 = L, channel 1 = R
            auto* bufferL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
            auto* bufferR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                float nextSample = random.nextFloat() * level;
                *bufferL++ = nextSample;
                *bufferR++ = nextSample;
            }

        }
    }
    // pink noise
    else if (pButton.getToggleState())
    {
        // the way that the PinkNoise class works, the pink noise ends up being
        // a bit lower in level than white noise. Doubling the level helps to make
        // the difference in volume between the two noise types less noticeable
        level *= 2;
        // check if stereo
        if (stereoB.getToggleState())
        {
            // generate stereo white noise, meaning separate noise values in each channel
            for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
            {
                auto* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
                for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
                    *buffer++ = nP.generate() * level;
            }
        }
        else
        {
            // channel 0 = L, channel 1 = R
            auto* bufferL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
            auto* bufferR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
            {
                float nextSample = nP.generate() * level;
                *bufferL++ = nextSample;
                *bufferR++ = nextSample;
            }

        }
    }
    // default, no output
    else
    {
        bufferToFill.clearActiveBufferRegion();
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()

    Logger::getCurrentLogger()->writeToLog("Releasing audio resources");
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::black);

    // You can add your drawing code here!
}



void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    titleLabel.setBoundsRelative(0.1, 0.045, 0.8, 0.15);

    wButton.setBoundsRelative(0.125, 0.235, 0.25, 0.15);
    pButton.setBoundsRelative(0.375, 0.235, 0.25, 0.15);
    offButton.setBoundsRelative(0.625, 0.235, 0.25, 0.15);

    stereoB.setBoundsRelative(0.3, 0.425, 0.4, 0.15);

    levelSlider.setBoundsRelative(0.1, 0.615, 0.8, 0.16);
    levelLabel.setBoundsRelative(0.1, 0.795, 0.8, 0.15);
}

void MainComponent::updateToggleState(Button* button, String name)
{
    auto state = button->getToggleState();
    String stateString = state ? "ON" : "OFF";

    Logger::outputDebugString(name + " Button changed to " + stateString);
}

