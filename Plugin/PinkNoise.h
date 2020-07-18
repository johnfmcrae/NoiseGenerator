/*
  ==============================================================================

    PinkNoise.h
    Created: 23 Jun 2020 1:31:08pm
    Author:  John

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PinkNoise
{
private:
    juce::Random noiseSrc;
    std::vector<float> pinkRows; // each row effectively holds an independednt random number generator
    float pinkRunSum; // running sum
    int pinkIndex; // the column index, incremented each sample
    int pinkIndexMask;
    float pinkNorm; // used to normalize the noise at the output

public:
    // constructor, overload to initialize with 12 rows, which worked out to be a
    // good number when testing in Octave
    PinkNoise(int numRows = 12);
    // generates pink noise one sample at a time
    float generate();
    // can  be used to reset the number of rows
    void setRows(int newRows);
};