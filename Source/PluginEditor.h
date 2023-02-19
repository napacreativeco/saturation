/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class SaturationAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SaturationAudioProcessorEditor (SaturationAudioProcessor&);
    ~SaturationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SaturationAudioProcessor& audioProcessor;

    juce::Slider inputGainSlider;
    juce::Slider freqStartSlider;
    juce::Slider freqEndSlider;
    //juce::Slider convLengthSlider;
    juce::Slider ratioSlider;
    juce::Slider thresholdSlider;
    juce::Slider outputGainSlider;

    OtherLookAndFeel otherLookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqStartAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqEndAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> convLengthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SaturationAudioProcessorEditor)
};
