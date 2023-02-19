/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SaturationAudioProcessor::SaturationAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "PARAMETERS", createParameters())
#endif
{

    //===========================
    // PRE GAIN
    //===========================
    auto& inputGain = processorChain.template get<0>();
    inputGain.setRampDurationSeconds(0.02);
    inputGain.setGainDecibels(3.0f);

    //===========================
    // FILTER START
    //===========================
    auto& filterStart = processorChain.template get<1>();

    filterStart.setMode(juce::dsp::LadderFilterMode::HPF12);
    filterStart.setCutoffFrequencyHz(400.0);
    filterStart.setResonance(0.4);
    filterStart.setDrive(1.0);

    //===========================
    // WAVESHAPER
    //===========================
    auto& waveshaper = processorChain.template get<2>();
    waveshaper.functionToUse = [](float x)
    {
        return std::tanh(x);
    };

    //===========================
    // FILTER END
    //===========================
    auto& filterEnd = processorChain.template get<3>();

    // Lightly attenuate High Frequency with LowPass12
    filterEnd.setMode(juce::dsp::LadderFilterMode::LPF12);
    filterEnd.setCutoffFrequencyHz(800.0);
    filterEnd.setResonance(0.5);
    filterEnd.setDrive(10.0);

    //===========================
    // CONVOLUTION
    //===========================
    auto dir = juce::File::getCurrentWorkingDirectory();
    int numTries = 0;

    while (!dir.getChildFile("IR").exists() && numTries++ < 15)
        dir = dir.getParentDirectory();

    auto& convolution = processorChain.template get<4>();

    convolution.loadImpulseResponse(dir.getChildFile("IR").getChildFile("cassette_recorder.wav"),
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::no,
        1024);

    //===========================
    // POST GAIN
    //===========================
    auto& outputGain = processorChain.template get<5>();
    outputGain.setRampDurationSeconds(0.02);
    outputGain.setGainDecibels(2.0f);
}

SaturationAudioProcessor::~SaturationAudioProcessor()
{
}

//==============================================================================
const juce::String SaturationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SaturationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SaturationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SaturationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SaturationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SaturationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SaturationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SaturationAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SaturationAudioProcessor::getProgramName (int index)
{
    return {};
}

void SaturationAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
// PREPARE TO PLAY
//==============================================================================
void SaturationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32> (getTotalNumOutputChannels());

    processorChain.prepare(spec);
}

void SaturationAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SaturationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


//==============================================================================
// PROCESS BLOCK
//==============================================================================
void SaturationAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
        
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> ctx(block);

    processorChain.process(ctx);
}

//==============================================================================
bool SaturationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SaturationAudioProcessor::createEditor()
{
    // return new SaturationAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void SaturationAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SaturationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// CREATE PARAMETERS
//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout SaturationAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // parameters in here
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // push back to add a new param to the vector
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FREQ", "Frequency", 600.0f, 1000.0f, 800.0f));

    return { params.begin(), params.end() };

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SaturationAudioProcessor();
}
