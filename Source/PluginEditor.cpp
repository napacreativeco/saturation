/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturationAudioProcessorEditor::SaturationAudioProcessorEditor (SaturationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 500);

    setLookAndFeel(&otherLookAndFeel);

    addAndMakeVisible(audioProcessor.waveViewer);

    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT", inputGainSlider);
    freqStartAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ_START", freqStartSlider);
    ratioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATIO", ratioSlider);
    thresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", thresholdSlider);
    freqEndAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ_END", freqEndSlider);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUTPUT", outputGainSlider);

    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqStartSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ratioSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    freqEndSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);

    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    freqStartSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    ratioSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    thresholdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    freqEndSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 30);

    addAndMakeVisible(inputGainSlider);
    addAndMakeVisible(freqStartSlider);
    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(freqEndSlider);
    addAndMakeVisible(outputGainSlider);
}

SaturationAudioProcessorEditor::~SaturationAudioProcessorEditor()
{
}

//==============================================================================
void SaturationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centredTop, 1);
}

void SaturationAudioProcessorEditor::resized()
{
    inputGainSlider.setBounds(0, 0, 100, 100);
    freqStartSlider.setBounds(100, 0, 100, 100);
    ratioSlider.setBounds(200, 0, 100, 100);
    thresholdSlider.setBounds(300, 0, 100, 100);
    freqEndSlider.setBounds(400, 0, 100, 100);
    outputGainSlider.setBounds(500, 0, 100, 100);

    audioProcessor.waveViewer.setBounds(220, 211, 159, 64);
}
