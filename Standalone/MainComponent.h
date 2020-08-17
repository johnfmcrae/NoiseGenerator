/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinkNoise.h"
#include "OldScoolLaF.h"

// JUCE 6 does not scope this automatically anymore
using namespace juce;

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
	//OldSchoolLookAndFeel oldSchoolLookAndFeel;
	OldScoolLaF LaF;
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

	// import rendered binary typeface

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};


//Typeface::Ptr getTypefaceForFont(const Font& font)
//{
//	Typeface::Ptr tf;
//	String faceName(font.getTypefaceName());
//	// Make requests for the default sans serif font use our
//	// FreeType hinted font instead.
//	if (faceName == Font::getDefaultSansSerifFontName())
//	{
//		// Create a new Font identical to the old one, then
//		// switch the name to our hinted font.
//		Font f(font);
//		// You'll need to know the exact name embedded in the font. There
//		// are a variety of free programs for retrieving this information.
//		f.setTypefaceName("font name goes here");
//		// Now get the hinted typeface.
//		tf = Typeface::createSystemTypefaceFor(Apple_II_Resource::Apple_II, Apple_II_Resource::Apple_IISize);
//	}
//	// If we got here without creating a new typeface
//	// then just use the default LookAndFeel behavior.
//	if (!tf)
//	{
//		tf = getTypefaceForFont(font);
//	}
//	return tf;
//}