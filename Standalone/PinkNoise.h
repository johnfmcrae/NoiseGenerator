/*
  ==============================================================================

    PinkNoise.h
    Created: 23 Jun 2020 1:31:08pm
    Last updated: 31 July 2020 5:47 pm
    Author:  John F. McRae

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PinkNoise
{
private:
    // random noise generator from the JUCE library
    juce::Random noiseSrc;
    // each row effectively holds an independent random number generator
    std::vector<float> pinkRows;
    // running sum for noise output
    float pinkRunSum;
    // the column index, incremented each sample
    int pinkIndex; 
    // the row mask, which ensures that the index of the pinkRows vector is never exceeded
    int pinkIndexMask;
    // used to normalize the noise at the output
    float pinkNorm;

public:
    // constructor, overload to initialize with 12 rows, which worked out to be a
    // good number when testing in Octave
    PinkNoise(int numRows = 12);
    // generates pink noise one sample at a time
    float generate();
    // can  be used to reset the number of rows
    void setRows(int newRows);
};