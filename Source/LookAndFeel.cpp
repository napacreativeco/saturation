/*
  ==============================================================================
    LookAndFeel.cpp
    Created: 14 Feb 2023 5:52:42am
    Author:  thisi
  ==============================================================================
*/

#include "LookAndFeel.h"
#include "PluginProcessor.h"

OtherLookAndFeel::OtherLookAndFeel()
{
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
}

void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    //g.setColour(juce::Colours::orange);
    //g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(juce::Colours::darkslategrey);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.13f;
    auto pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(juce::Colours::ghostwhite);
    g.fillPath(p);
}


void OtherLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& toggle, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    g.setColour(juce::Colours::red);
    g.fillRoundedRectangle(toggle.getBounds().reduced(5.f, 5.f).toFloat(), 5.f);
}