/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.1/Delay.h"
#include "StkLite-4.6.1/BiQuad.h"
#include "Defines.h"


//==============================================================================
/**
*/
class ColemanJP03DelayAudioProcessorEditor  : public juce::AudioProcessorEditor,
public juce::Slider::Listener, public juce::Button::Listener,
public juce::Timer
{
public:
    ColemanJP03DelayAudioProcessorEditor (ColemanJP03DelayAudioProcessor&);
    ~ColemanJP03DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void buttonStateChanged(juce::Button* button) override;
    void buttonClicked(juce::Button* button) override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ColemanJP03DelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColemanJP03DelayAudioProcessorEditor)
    
    juce::Slider leftDelaySlider;
    juce::Slider rightDelaySlider;
    
    juce::Slider leftFeedbackSlider;
    juce::Slider rightFeedbackSlider;
    
    juce::Slider leftWetDrySlider;
    juce::Slider rightWetDrySlider;
    
    juce::Slider leftFilterSlider;
    juce::Slider rightFilterSlider;
    
    juce::ToggleButton matchLRToggle { "Match L/R" };
    
    juce::Label leftLabel;
    juce::Label rightLabel;
    
    juce::Label timeLabel;
    juce::Label feedbackLabel;
    juce::Label dryWetLabel;
    
    juce::Label leftLowCutFcLabel;
    juce::Label leftHighCutFcLabel;
    juce::Label rightLowCutFcLabel;
    juce::Label rightHighCutFcLabel;
    
    enum parameterMap {
        leftDelayMs,
        rightDelayMs,
        leftFeedback,
        rightFeedback,
        leftDryWet,
        rightDryWet,
        leftHighPassFc,
        rightHighPassFc,
        leftLowPassFc,
        rightLowPassFc,
        matchLR
    };
    
    void makeDelaySlider(int x, juce::Slider& slider);
    void makeFeedbackSlider(int x, juce::Slider& slider);
    void makeFilterSlider(int x, juce::Slider& slider);
    void makeWetDrySlider(int x, juce::Slider& slider);
    void makeLRLabel(int x, juce::Label& label, std::string labelText);
    void makeCenterLabel(int y, juce::Label& label, std::string labelText);
    void makeFcValLabel(int x, juce::Label& label, juce::Justification justification);
    void setParamFromSlider(juce::Slider& slider, parameterMap paramNum);
    void setParamFromFilterSlider(juce::Slider& slider, parameterMap lowFc, parameterMap highFc);
    void setSliderFromParam(juce::Slider& slider, parameterMap paramNum,
        const juce::Array<juce::AudioProcessorParameter *> &parameters);
    void setFilterSliderFromParam(juce::Slider& slider, parameterMap lowFc,
        parameterMap highFc, const juce::Array<juce::AudioProcessorParameter *> &parameters);
};
