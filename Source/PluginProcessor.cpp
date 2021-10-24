/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ColemanJP03DelayAudioProcessor::ColemanJP03DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    juce::NormalisableRange<float> delayRange = juce::NormalisableRange<float>(
        DELAY_LENGTH_MS_MIN, DELAY_LENGTH_MS_MAX, DELAY_LENGTH_MS_INTERVAL,
        DELAY_LENGTH_MS_SKEW);
    addParameter(leftDelayMsParam = new juce::AudioParameterFloat("leftDelayMs",
                                          "Left Delay (ms)",
                                          delayRange,
                                          DELAY_LENGTH_MS_DEFAULT));
    addParameter(rightDelayMsParam = new juce::AudioParameterFloat("rightDelayMs",
                                          "Right Delay (ms)",
                                          delayRange,
                                          DELAY_LENGTH_MS_DEFAULT));
    
    addParameter(leftFeedbackParam = new juce::AudioParameterFloat("leftFeedback",
                                            "Left Feedback",
                                            FEEDBACK_MIN,
                                            FEEDBACK_MAX,
                                            FEEDBACK_DEFAULT));
    addParameter(rightFeedbackParam = new juce::AudioParameterFloat("rightFeedback",
                                            "Right Feedback",
                                            FEEDBACK_MIN,
                                            FEEDBACK_MAX,
                                            FEEDBACK_DEFAULT));
    
    addParameter(leftDryWetParam = new juce::AudioParameterFloat("leftDryWet",
                                            "Left Dry/Wet",
                                            DRY_WET_MIN,
                                            DRY_WET_MAX,
                                            DRY_WET_DEFAULT));
    addParameter(rightDryWetParam = new juce::AudioParameterFloat("rightDryWet",
                                            "Right Dry/Wet",
                                             DRY_WET_MIN,
                                             DRY_WET_MAX,
                                             DRY_WET_DEFAULT));

    
    juce::NormalisableRange<float> fcRange = juce::NormalisableRange<float>(
        FILTER_FC_MIN, FILTER_FC_MAX, FILTER_FC_INTERVAL, FILTER_FC_SKEW);
    
    addParameter(leftHighPassFcParam = new juce::AudioParameterFloat("leftHighPassCutFc",
                                            "Left Low Cut Fc",
                                            fcRange,
                                            LOW_CUT_DEFAULT_FC));
    addParameter(rightHighPassFcParam = new juce::AudioParameterFloat("rightHighPassCutFc",
                                            "Right Low Cut Fc",
                                            fcRange,
                                            LOW_CUT_DEFAULT_FC));
    addParameter(leftLowPassFcParam = new juce::AudioParameterFloat("leftLowPassCutFc",
                                            "Left High Cut Fc",
                                            fcRange,
                                            HIGH_CUT_DEFAULT_FC));
    addParameter(rightLowPassFcParam = new juce::AudioParameterFloat("rightLowPassCutFc",
                                            "Right High Cut Fc",
                                            fcRange,
                                            HIGH_CUT_DEFAULT_FC));
    
    addParameter(matchLRParam = new juce::AudioParameterBool("matchLR",
                                                             "Match L/R",
                                                             false));
}

ColemanJP03DelayAudioProcessor::~ColemanJP03DelayAudioProcessor()
{
}

//==============================================================================
const juce::String ColemanJP03DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ColemanJP03DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ColemanJP03DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ColemanJP03DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ColemanJP03DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ColemanJP03DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ColemanJP03DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ColemanJP03DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ColemanJP03DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void ColemanJP03DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ColemanJP03DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    fs = sampleRate;
    
    unsigned long samps = calcDelaySampsFromMs(DELAY_LENGTH_MS_MAX);
    leftDelay.setMaximumDelay(samps);
    rightDelay.setMaximumDelay(samps);
}

unsigned long ColemanJP03DelayAudioProcessor::calcDelaySampsFromMs(float ms) {
    return std::ceil(ms*(fs/1000.0));
}

void ColemanJP03DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ColemanJP03DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (
//        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
     layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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


// convert percentage to gain for feedback (using dB scale)
float determineFeedbackGain(float percent) {
    if (percent == 0)
        return 0;
    float scaled_val = (100-percent)/5.0;
    return pow(10, -scaled_val/20.0);
}

void ColemanJP03DelayAudioProcessor::calcAlgorithmParams() {
    /* Filters */
    float coeffsHPL[5], coeffsLPL[5], coeffsHPR[5], coeffsLPR[5];
    
    Mu45FilterCalc::calcCoeffsHPF(coeffsHPL,
                                  leftHighPassFcParam->get(),
                                  LOW_CUT_Q, fs);
    leftHighPass.setCoefficients(coeffsHPL[0],coeffsHPL[1],
                                 coeffsHPL[2],coeffsHPL[3],
                                 coeffsHPL[4]);
    
    Mu45FilterCalc::calcCoeffsLPF(coeffsLPL,
                                  leftLowPassFcParam->get(),
                                  HIGH_CUT_Q, fs);
    leftLowPass.setCoefficients(coeffsLPL[0],coeffsLPL[1],
                                coeffsLPL[2],coeffsLPL[3],
                                coeffsLPL[4]);
    
    Mu45FilterCalc::calcCoeffsHPF(coeffsHPR,
                                  rightHighPassFcParam->get(),
                                  LOW_CUT_Q, fs);
    rightHighPass.setCoefficients(coeffsHPR[0],coeffsHPR[1],
                                 coeffsHPR[2],coeffsHPR[3],
                                 coeffsHPR[4]);
    
    Mu45FilterCalc::calcCoeffsLPF(coeffsLPR,
                                  rightLowPassFcParam->get(),
                                  HIGH_CUT_Q, fs);
    rightLowPass.setCoefficients(coeffsLPR[0],coeffsLPR[1],
                                coeffsLPR[2],coeffsLPR[3],
                                coeffsLPR[4]);


    /* Wet/Dry */
    leftWetGain = leftDryWetParam->get()/100.0;
    leftDryGain = 1 - leftDryWetParam->get()/100.0;
    rightWetGain = rightDryWetParam->get()/100.0;
    rightDryGain = 1 - rightDryWetParam->get()/100.0;

    
    /* Delay Length */
    leftDelay.setDelay(calcDelaySampsFromMs(leftDelayMsParam->get()));
    rightDelay.setDelay(calcDelaySampsFromMs(rightDelayMsParam->get()));
    
    /* Feedback */
    // use db scale under the hood
    // from 1 to 100 -> scaled from -20 to 0 dB loss
    leftFeedbackGain = determineFeedbackGain(leftFeedbackParam->get());
    rightFeedbackGain = determineFeedbackGain(rightFeedbackParam->get());

}

void ColemanJP03DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    calcAlgorithmParams();
    
    auto* leftChannelData = buffer.getWritePointer(0);
    auto* rightChannelData = buffer.getWritePointer(1);
    
    for (int samp = 0; samp < buffer.getNumSamples(); samp ++) {
        
        float leftFeedbackDelayInput = leftDelay.nextOut();
        leftFeedbackDelayInput *= leftFeedbackGain;
        leftFeedbackDelayInput = leftLowPass.tick(leftFeedbackDelayInput);
        leftFeedbackDelayInput = leftHighPass.tick(leftFeedbackDelayInput);
        leftDelay.tick(leftChannelData[samp] + leftFeedbackDelayInput);
        
        leftChannelData[samp] = leftDryGain*leftChannelData[samp] + leftFeedbackGain*leftWetGain*leftDelay.nextOut();
        
        float rightFeedbackDelayInput = rightDelay.nextOut();
        rightFeedbackDelayInput *= rightFeedbackGain;
        rightFeedbackDelayInput = rightLowPass.tick(rightFeedbackDelayInput);
        rightFeedbackDelayInput = rightHighPass.tick(rightFeedbackDelayInput);
        rightDelay.tick(rightChannelData[samp] + rightFeedbackDelayInput);

        rightChannelData[samp] = rightDryGain*rightChannelData[samp] + rightFeedbackGain*rightWetGain*rightDelay.nextOut();
    }
}

//==============================================================================
bool ColemanJP03DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ColemanJP03DelayAudioProcessor::createEditor()
{
    return new ColemanJP03DelayAudioProcessorEditor (*this);
}

//==============================================================================
// saves the current state of the float parameters
void ColemanJP03DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    DBG("-- SAVING STATE INFO --");
    juce::XmlElement xml ("Parameters");
    for (int i = 0; i < getParameters().size() - 1; ++i) // - 1 to ignore matchLRParam (an AudioParameterBool)
    {
        juce::AudioParameterFloat* param = (juce::AudioParameterFloat*)getParameters().getUnchecked(i);
        juce::XmlElement* paramElement = new juce::XmlElement ("parameter" + juce::String(std::to_string(i)));
        paramElement->setAttribute ("value", param->get());
        xml.addChildElement (paramElement);
    }

    DBG(xml.toString());
    copyXmlToBinary (xml, destData);
}

// sets the state based on previously saved XML.
// when the plugin loads for the first time, the defaults will get saved into XML
// and then read buy this function to set the GUI
void ColemanJP03DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    DBG("-- READING SAVED STATE --");
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    DBG(xmlState->toString());
    
    if (xmlState->hasTagName ("Parameters")) // read Parameters tag
    {
        juce::AudioParameterFloat* param;
        for (auto* element : xmlState->getChildIterator()) // loop through the saved parameter values and update them
        {
            int paramNum = std::stoi(element->getTagName().substring(9).toStdString()); // chops off beginnging "parameter"
            param = (juce::AudioParameterFloat*) getParameters().getUnchecked(paramNum);
            *param = element->getDoubleAttribute("value"); // set parameter value
        }
    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ColemanJP03DelayAudioProcessor();
}
