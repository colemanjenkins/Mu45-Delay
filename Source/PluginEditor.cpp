/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

/* Helper functions to avoid repetitive code for L/R knobs and labels */
void ColemanJP03DelayAudioProcessorEditor::makeDelaySlider(int x, juce::Slider& slider) {
    slider.setBounds(x, 3.5*UNIT_LENGTH_Y, 5.5*UNIT_LENGTH_X, 5*UNIT_LENGTH_Y);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setTextValueSuffix(DELAY_SUFFIX);
    slider.setRange(DELAY_LENGTH_MS_MIN, DELAY_LENGTH_MS_MAX, DELAY_LENGTH_MS_INTERVAL);
    slider.setSkewFactor(DELAY_LENGTH_MS_SKEW);
//    slider.setValue(DELAY_LENGTH_MS_DEFAULT);
    slider.addListener(this);
    addAndMakeVisible(slider);
}

void ColemanJP03DelayAudioProcessorEditor::makeFeedbackSlider(int x, juce::Slider& slider) {
    slider.setBounds(x, 8.5*UNIT_LENGTH_Y, 5.5*UNIT_LENGTH_X, 5*UNIT_LENGTH_Y);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setTextValueSuffix(FEEDBACK_SUFFIX);
    slider.setRange(FEEDBACK_MIN, FEEDBACK_MAX, FEEDBACK_INTERVAL);
//    slider.setValue(FEEDBACK_DEFAULT);
    slider.addListener(this);
    addAndMakeVisible(slider);
}

void ColemanJP03DelayAudioProcessorEditor::makeFilterSlider(int x, juce::Slider& slider) {
    slider.setBounds(x, 14*UNIT_LENGTH_Y, 9*UNIT_LENGTH_X, 3*UNIT_LENGTH_Y);
    slider.setSliderStyle(juce::Slider::TwoValueHorizontal);
//    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
//    slider.setTextValueSuffix(FILTER_SUFFIX);
    slider.setTextBoxStyle (juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange(FILTER_FC_MIN, FILTER_FC_MAX, FILTER_FC_INTERVAL);
    slider.setMinValue(LOW_CUT_DEFAULT_FC);
    slider.setMaxValue(HIGH_CUT_DEFAULT_FC);
    slider.setSkewFactor(FILTER_FC_SKEW);
    slider.addListener(this);
    addAndMakeVisible(slider);
}

void ColemanJP03DelayAudioProcessorEditor::makeWetDrySlider(int x, juce::Slider& slider) {
    slider.setBounds(x, 17.5*UNIT_LENGTH_Y, 4*UNIT_LENGTH_X, 5*UNIT_LENGTH_Y);
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    slider.setTextValueSuffix(DRY_WET_SUFFIX);
    slider.setRange(DRY_WET_MIN, DRY_WET_MAX, DRY_WET_INTERVAL);
//    slider.setValue(DRY_WET_DEFAULT);
    slider.addListener(this);
    addAndMakeVisible(slider);
}

void ColemanJP03DelayAudioProcessorEditor::makeLRLabel(int x, juce::Label& label, std::string labelText) {
    label.setText(labelText, juce::dontSendNotification);
    label.setBounds(x, 1*UNIT_LENGTH_Y, 4*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font (20.0f, juce::Font::bold));
    addAndMakeVisible(label);
}

void ColemanJP03DelayAudioProcessorEditor::makeCenterLabel(int y, juce::Label& label, std::string labelText) {
    label.setText(labelText, juce::dontSendNotification);
    label.setBounds(9.5*UNIT_LENGTH_X, y, 5*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y);
    label.setJustificationType(juce::Justification::centred);
    label.setFont(juce::Font (20.0f, juce::Font::bold));
    addAndMakeVisible(label);
}


void ColemanJP03DelayAudioProcessorEditor::makeFcValLabel(int x, juce::Label& label,
                                    juce::Justification justification) {
//    label.setText(labelText, juce::dontSendNotification);
    label.setBounds(x, 16*UNIT_LENGTH_Y, 3*UNIT_LENGTH_X, 1*UNIT_LENGTH_Y);
    label.setJustificationType(justification);
    label.setFont(juce::Font (16.0f));
    addAndMakeVisible(label);
}

//==============================================================================
ColemanJP03DelayAudioProcessorEditor::ColemanJP03DelayAudioProcessorEditor (ColemanJP03DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (CONTAINER_WIDTH, CONTAINER_HEIGHT);
    
    /* LR Toggle */
    matchLRToggle.setBounds(10.25*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y, 6*UNIT_LENGTH_X, 2*UNIT_LENGTH_Y);
    matchLRToggle.setToggleState(MATCH_LR_DEFAULT, juce::dontSendNotification);
    matchLRToggle.addListener(this);
    addAndMakeVisible(matchLRToggle);
    
    /* Delay Sliders */
    makeDelaySlider(3*UNIT_LENGTH_X, leftDelaySlider);
    makeDelaySlider(15.5*UNIT_LENGTH_X, rightDelaySlider);
    
    /* Feedback Sliders */
    makeFeedbackSlider(3*UNIT_LENGTH_X, leftFeedbackSlider);
    makeFeedbackSlider(15.5*UNIT_LENGTH_X, rightFeedbackSlider);
    
    /* Filter Sliders*/
    makeFilterSlider(1*UNIT_LENGTH_X, leftFilterSlider);
    makeFilterSlider(14*UNIT_LENGTH_X, rightFilterSlider);
    
    /* Dry Wet Sliders */
    makeWetDrySlider(3.75*UNIT_LENGTH_X, leftWetDrySlider);
    makeWetDrySlider(16.25*UNIT_LENGTH_X, rightWetDrySlider);
    
    /* Labels */
    makeLRLabel(3.75*UNIT_LENGTH_X, leftLabel, "Left");
    makeLRLabel(16.25*UNIT_LENGTH_X, rightLabel, "Right");
    makeCenterLabel(4.5*UNIT_LENGTH_Y, timeLabel, "Time");
    makeCenterLabel(9.5*UNIT_LENGTH_Y, feedbackLabel, "Feedback");
    makeCenterLabel(18.5*UNIT_LENGTH_Y, dryWetLabel, "Wet/Dry");
    
    makeFcValLabel(1*UNIT_LENGTH_X, leftLowCutFcLabel, juce::Justification::bottomLeft);
    makeFcValLabel(7*UNIT_LENGTH_X, leftHighCutFcLabel, juce::Justification::bottomRight);
    makeFcValLabel(14*UNIT_LENGTH_X, rightLowCutFcLabel, juce::Justification::bottomLeft);
    makeFcValLabel(20*UNIT_LENGTH_X, rightHighCutFcLabel, juce::Justification::bottomRight);
    
    startTimer(20); // update GUI from parameters every 20 ms, useful for param automation and saving state

}

ColemanJP03DelayAudioProcessorEditor::~ColemanJP03DelayAudioProcessorEditor()
{
}

/* Functions to avoid repetitive code in setting sliders and params from each other */
void ColemanJP03DelayAudioProcessorEditor::setSliderFromParam(juce::Slider& slider, parameterMap paramNum, const juce::Array<juce::AudioProcessorParameter *> &parameters) {
    
    juce::AudioParameterFloat* param = (juce::AudioParameterFloat*)parameters.getUnchecked(paramNum);
    slider.setValue(param->get(), juce::dontSendNotification);
}

void ColemanJP03DelayAudioProcessorEditor::setFilterSliderFromParam(juce::Slider& slider, parameterMap lowFc,
        parameterMap highFc, const juce::Array<juce::AudioProcessorParameter *> &parameters) {
    
    juce::AudioParameterFloat* lowParam = (juce::AudioParameterFloat*)parameters.getUnchecked(lowFc);
    slider.setMinValue(lowParam->get(), juce::dontSendNotification);
    
    juce::AudioParameterFloat* highParam = (juce::AudioParameterFloat*)parameters.getUnchecked(highFc);
    slider.setMaxValue(highParam->get(), juce::dontSendNotification);
}


void ColemanJP03DelayAudioProcessorEditor::setParamFromSlider(juce::Slider& slider, parameterMap paramNum) {
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* audioParam = (juce::AudioParameterFloat*)params.getUnchecked(paramNum);
    *audioParam = slider.getValue();
}

void ColemanJP03DelayAudioProcessorEditor::setParamFromFilterSlider(juce::Slider& slider, parameterMap lowFc, parameterMap highFc) {
    auto& params = processor.getParameters();
    juce::AudioParameterFloat* lowFcParam = (juce::AudioParameterFloat*)params.getUnchecked(lowFc);
    juce::AudioParameterFloat* highFcParam = (juce::AudioParameterFloat*)params.getUnchecked(highFc);
    *lowFcParam = slider.getMinValue();
    *highFcParam = slider.getMaxValue();
}

// helper function to display frequency with 3 sig figs and adaptive units
std::string fc_tostring(float val) {
    int num_digits = (int) ceil(log(val) / log(10));
    
    std::string output;
    if (num_digits == 5) {
        int vals = (int) round(val / 100.0);
        output = std::to_string(vals / 10) + ".";
        output += std::to_string(vals % 10) + FILTER_SUFFIX_KHZ;
    } else if (num_digits == 4) {
        int vals = (int) round(val / 10.0);
        output = std::to_string(vals / 100) + ".";
        output += std::to_string((vals % 100) / 10);
        output += std::to_string(vals % 10)+ FILTER_SUFFIX_KHZ;
    } else if (num_digits == 3) {
        int vals = (int) round(val);
        output = std::to_string(vals) + FILTER_SUFFIX_HZ;
    } else if (num_digits == 2) {
        int vals = (int) round(val * 10.0);
        output = std::to_string(vals / 10) + ".";
        output += std::to_string(vals % 10) + FILTER_SUFFIX_HZ;
    } else {
        output = "Error";
    }
    
    return output;
}

// set parameters from slider changes
void ColemanJP03DelayAudioProcessorEditor::sliderValueChanged(juce::Slider *slider) {
    bool matched = matchLRToggle.getToggleState();
    
    if (slider == &leftDelaySlider) {
        setParamFromSlider(leftDelaySlider, leftDelayMs);
        if (matched) rightDelaySlider.setValue(leftDelaySlider.getValue());
        
    } else if (slider == &rightDelaySlider) {
        setParamFromSlider(rightDelaySlider, rightDelayMs);
        if (matched) leftDelaySlider.setValue(rightDelaySlider.getValue());
        
    } else if (slider == &leftFeedbackSlider) {
        setParamFromSlider(leftFeedbackSlider, leftFeedback);
        if (matched) rightFeedbackSlider.setValue(leftFeedbackSlider.getValue());
        
    } else if (slider == &rightFeedbackSlider) {
        setParamFromSlider(rightFeedbackSlider, rightFeedback);
        if (matched) leftFeedbackSlider.setValue(rightFeedbackSlider.getValue());
        
    } else if (slider == &leftWetDrySlider) {
        setParamFromSlider(leftWetDrySlider, leftDryWet);
        if (matched) rightWetDrySlider.setValue(leftWetDrySlider.getValue());
        
    } else if (slider == &rightWetDrySlider) {
        setParamFromSlider(rightWetDrySlider, rightDryWet);
        if (matched) leftWetDrySlider.setValue(rightWetDrySlider.getValue());
        
    } else if (slider == &leftFilterSlider) {
        setParamFromFilterSlider(leftFilterSlider, leftHighPassFc, leftLowPassFc);
        leftLowCutFcLabel.setText(fc_tostring(leftFilterSlider.getMinValue()),
                                  juce::dontSendNotification);
        leftHighCutFcLabel.setText(fc_tostring(leftFilterSlider.getMaxValue()),
                                   juce::dontSendNotification);
        if (matched)
            rightFilterSlider.setMinAndMaxValues(leftFilterSlider.getMinValue(),
                                                 leftFilterSlider.getMaxValue());
        
    } else if (slider == &rightFilterSlider) {
        setParamFromFilterSlider(rightFilterSlider, rightHighPassFc, rightLowPassFc);
        rightLowCutFcLabel.setText(fc_tostring(rightFilterSlider.getMinValue()),
                                  juce::dontSendNotification);
        rightHighCutFcLabel.setText(fc_tostring(rightFilterSlider.getMaxValue()),
                                   juce::dontSendNotification);

        if (matched)
            leftFilterSlider.setMinAndMaxValues(rightFilterSlider.getMinValue(),
                                                 rightFilterSlider.getMaxValue());

    }
}

// Set sliders from paramter changes
void ColemanJP03DelayAudioProcessorEditor::timerCallback() {
    auto& params = processor.getParameters();
    
    setSliderFromParam(leftDelaySlider, leftDelayMs, params);
    setSliderFromParam(rightDelaySlider, rightDelayMs, params);
    setSliderFromParam(leftFeedbackSlider, leftFeedback, params);
    setSliderFromParam(rightFeedbackSlider, rightFeedback, params);
    
    setFilterSliderFromParam(leftFilterSlider, leftHighPassFc, leftLowPassFc, params);
    setFilterSliderFromParam(rightFilterSlider, rightHighPassFc, rightLowPassFc, params);
    
    setSliderFromParam(leftWetDrySlider, leftDryWet, params);
    setSliderFromParam(rightWetDrySlider, rightDryWet, params);
}

// match right side to left when matchLR is toggled
void ColemanJP03DelayAudioProcessorEditor::buttonStateChanged(juce::Button *button) {
    if (matchLRToggle.getToggleState()) {
        rightDelaySlider.setValue(leftDelaySlider.getValue());
        rightFeedbackSlider.setValue(leftFeedbackSlider.getValue());
        rightWetDrySlider.setValue(leftWetDrySlider.getValue());
        rightFilterSlider.setMinAndMaxValues(leftFilterSlider.getMinValue(),
                                             leftFilterSlider.getMaxValue());
    }
}

void ColemanJP03DelayAudioProcessorEditor::buttonClicked(juce::Button *button) {
    
}

//==============================================================================
void ColemanJP03DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
    g.setColour(juce::Colours::grey);
    
    // lines in between text and rotary sliders
                // inside                               // outside
    // time
    g.drawLine(10.5*UNIT_LENGTH_X, 5.5*UNIT_LENGTH_Y, 8.25*UNIT_LENGTH_X, 5.5*UNIT_LENGTH_Y, 2.0f); // left
    g.drawLine(13.5*UNIT_LENGTH_X, 5.5*UNIT_LENGTH_Y, 15.75*UNIT_LENGTH_X, 5.5*UNIT_LENGTH_Y, 2.0f); // right
    // feedback
    g.drawLine(9.75*UNIT_LENGTH_X, 10.5*UNIT_LENGTH_Y, 8.25*UNIT_LENGTH_X, 10.5*UNIT_LENGTH_Y, 2.0f); // left
    g.drawLine(14.25*UNIT_LENGTH_X, 10.5*UNIT_LENGTH_Y, 15.75*UNIT_LENGTH_X, 10.5*UNIT_LENGTH_Y, 2.0f); // right
    // dry/wet
    g.drawLine(10*UNIT_LENGTH_X, 19.5*UNIT_LENGTH_Y, 8*UNIT_LENGTH_X, 19.5*UNIT_LENGTH_Y, 2.0f); // left
    g.drawLine(14*UNIT_LENGTH_X, 19.5*UNIT_LENGTH_Y, 16*UNIT_LENGTH_X, 19.5*UNIT_LENGTH_Y, 2.0f); // right

    // filter labels
    g.setColour(juce::Colours::white);
    g.drawText("Low Cut", 1*UNIT_LENGTH_X, 13.5*UNIT_LENGTH_Y, 3*UNIT_LENGTH_X, 1*UNIT_LENGTH_Y, juce::Justification::bottomLeft);
    g.drawText("High Cut", 7*UNIT_LENGTH_X, 13.5*UNIT_LENGTH_Y, 3*UNIT_LENGTH_X, 1*UNIT_LENGTH_Y, juce::Justification::bottomRight);
    
    g.drawText("Low Cut", 14*UNIT_LENGTH_X, 13.5*UNIT_LENGTH_Y, 3*UNIT_LENGTH_X, 1*UNIT_LENGTH_Y, juce::Justification::bottomLeft);
    g.drawText("High Cut", 20*UNIT_LENGTH_X, 13.5*UNIT_LENGTH_Y, 3*UNIT_LENGTH_X, 1*UNIT_LENGTH_Y, juce::Justification::bottomRight);
}

void ColemanJP03DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
