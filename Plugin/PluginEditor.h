/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
// add this so I don't have to scope the juce stuff everytime
using namespace juce;

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
/**
*/
class NoiseGeneratorPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    // Radio button IDs for the white/pink/off radio buttons
    enum RadioButtonIds
    {
        NoiseButtons = 1001
    };

    NoiseGeneratorPluginAudioProcessorEditor (NoiseGeneratorPluginAudioProcessor&);
    ~NoiseGeneratorPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void updateToggleState(Button* button, String name);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoiseGeneratorPluginAudioProcessor& audioProcessor;

    // look and feel class declaration
    OldSchoolLookAndFeel oldSchoolLookAndFeel;
    // GUI object declarations
    TextButton wButton;
    TextButton pButton;
    TextButton offButton;
    Slider levelSlider;
    Label titleLabel;
    Label levelLabel;

    // set OldSchoolLookAndFeel colours here
    void setupOldSchoolAndFeelColours(LookAndFeel& laf);

public:
    //  std::unique_ptr < juce::AudioProcessorValueTreeState::SliderAttachment > sliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> levelAttach;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> wAttach;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> pAttach;
    std::unique_ptr <AudioProcessorValueTreeState::ButtonAttachment> offAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGeneratorPluginAudioProcessorEditor)
};
