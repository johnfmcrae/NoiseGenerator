/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinkNoise.h"

//==============================================================================
/*
    Define the custom look and feel for the GUI.
    General idea here is some kind of old school computer vibe
*/
class OldSchoolLookAndFeel : public LookAndFeel_V4
{
public:
    OldSchoolLookAndFeel()
    {

    }

    // pull code from square look and feel demo
    void drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto baseColour = backgroundColour.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 0.9f : 0.5f);

        if (isButtonDown || isMouseOverButton)
            baseColour = baseColour.contrasting(isButtonDown ? 0.2f : 0.1f);

        auto width = button.getWidth() - 1.0f;
        auto height = button.getHeight() - 1.0f;

        if (width > 0 && height > 0)
        {
            g.setGradientFill(ColourGradient::vertical(baseColour, 0.0f,
                baseColour.darker(0.1f), height));

            g.fillRect(button.getLocalBounds());
        }
        g.setFont(Font(12.0f, Font::bold));
    }

    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const Slider::SliderStyle style, Slider& slider) override
    {
        g.fillAll(slider.findColour(Slider::backgroundColourId));

        if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
        {
            Path p;

            if (style == Slider::LinearBarVertical)
                p.addRectangle((float)x, sliderPos, (float)width, 1.0f + height - sliderPos);
            else
                p.addRectangle((float)x, (float)y, sliderPos - x, (float)height);

            auto baseColour = slider.findColour(Slider::rotarySliderFillColourId)
                .withMultipliedSaturation(slider.isEnabled() ? 1.0f : 0.5f)
                .withMultipliedAlpha(0.8f);

            g.setColour(baseColour);
            g.fillPath(p);

            auto lineThickness = jmin(15.0f, jmin(width, height) * 0.45f) * 0.1f;
            g.drawRect(slider.getLocalBounds().toFloat(), lineThickness);
        }
    }
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    enum RadioButtonIds
    {
        NoiseButtons = 1001
    };

    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;

    //==============================================================================
    void updateToggleState(Button* button, String name);

private:
    //==============================================================================
    // Your private member variables go here...
    OldSchoolLookAndFeel oldSchoolLookAndFeel;
    Random random; // class of type Random for use in white noise generation
    PinkNoise nP;
    TextButton wButton;
    TextButton pButton;
    TextButton offButton;
    TextButton stereoB;
    Slider levelSlider;
    Label titleLabel;
    Label levelLabel;

    // set OldSchoolLookAndFeel colours here
    void setupOldSchoolAndFeelColours(LookAndFeel& laf)
    {
        auto baseColour = Colours::green;

        laf.setColour(Slider::thumbColourId, Colour::greyLevel(0.95f));
        laf.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
        laf.setColour(Slider::rotarySliderFillColourId, baseColour);
        laf.setColour(Slider::rotarySliderOutlineColourId, Colours::black);
        laf.setColour(Slider::trackColourId, Colours::white);

        laf.setColour(TextButton::buttonColourId, Colours::black);
        laf.setColour(TextButton::textColourOffId, baseColour);

        laf.setColour(TextButton::buttonOnColourId, laf.findColour(TextButton::textColourOffId));
        laf.setColour(TextButton::textColourOnId, laf.findColour(TextButton::buttonColourId));
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
