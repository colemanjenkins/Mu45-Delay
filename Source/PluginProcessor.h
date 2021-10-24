/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.1/Delay.h"
#include "StkLite-4.6.1/BiQuad.h"
#include "Defines.h"

//==============================================================================
/**
*/
class ColemanJP03DelayAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ColemanJP03DelayAudioProcessor();
    ~ColemanJP03DelayAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColemanJP03DelayAudioProcessor)
    
    /* User Params */
    juce::AudioParameterFloat* leftDelayMsParam;
    juce::AudioParameterFloat* rightDelayMsParam;
    
    juce::AudioParameterFloat* leftFeedbackParam;
    juce::AudioParameterFloat* rightFeedbackParam;
    
    juce::AudioParameterFloat* leftDryWetParam;
    juce::AudioParameterFloat* rightDryWetParam;

    juce::AudioParameterFloat* leftHighPassFcParam;
    juce::AudioParameterFloat* leftLowPassFcParam;
    juce::AudioParameterFloat* rightHighPassFcParam;
    juce::AudioParameterFloat* rightLowPassFcParam;
    
    juce::AudioParameterBool* matchLRParam;
    
    /* Algorithm Params, Filters, and Delays*/
    stk::Delay leftDelay;
    stk::Delay rightDelay;

    stk::BiQuad leftHighPass;
    stk::BiQuad leftLowPass;
    stk::BiQuad rightHighPass;
    stk::BiQuad rightLowPass;
    
    float fs;
    float leftWetGain;
    float leftDryGain;
    float rightWetGain;
    float rightDryGain;
    float leftFeedbackGain;
    float rightFeedbackGain;
    
    void calcAlgorithmParams();
    unsigned long calcDelaySampsFromMs(float ms);
};
