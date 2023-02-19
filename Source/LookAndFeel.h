/*
  ==============================================================================
    LookAndFeel.h
    Created: 14 Feb 2023 5:52:42am
    Author:  thisi
  ==============================================================================
*/

#pragma once
#include "PluginProcessor.h"

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&);

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggle, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OtherLookAndFeel)
};