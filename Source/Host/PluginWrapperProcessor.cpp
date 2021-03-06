/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginWrapperProcessor.h"
#include "../CabbageUtils.h"


//==============================================================================
PluginWrapper::PluginWrapper(AudioPluginInstance* instance)
{
    isBypassed = false;
    isMuted = false;
    updateCounter = 0;
    vstInstance = instance;
    if(!vstInstance)
        assert(0);

    pluginDesc = instance->getPluginDescription();

}

PluginWrapper::~PluginWrapper()
{
}

void PluginWrapper::fillInPluginDescription(PluginDescription & mdescription) const
{
    mdescription.name = pluginDesc.name;
    mdescription.numInputChannels = pluginDesc.numInputChannels;
    mdescription.numOutputChannels = pluginDesc.numOutputChannels;
    mdescription.uid = pluginDesc.uid;
    mdescription.category = pluginDesc.category;
    mdescription.descriptiveName = pluginDesc.descriptiveName;
    mdescription.fileOrIdentifier = pluginDesc.fileOrIdentifier;
    mdescription.isInstrument = pluginDesc.isInstrument;
    mdescription.pluginFormatName = pluginDesc.pluginFormatName;
    mdescription.version = pluginDesc.version;
}
//==============================================================================
const String PluginWrapper::getName() const
{
    if(vstInstance)
        return vstInstance->getName();
}

int PluginWrapper::getNumParameters()
{
    if(vstInstance)
        return vstInstance->getNumParameters();
    else
        return 0;
}

float PluginWrapper::getParameter (int index)
{
    if(vstInstance)
        return vstInstance->getParameter(index);
    return 0.0f;
}

void PluginWrapper::setParameter (int index, float newValue)
{
    if(vstInstance)
        vstInstance->setParameter(index, newValue);
}

const String PluginWrapper::getParameterName (int index)
{
    if(vstInstance)
        return vstInstance->getParameterName(index);
    return String();
}

const String PluginWrapper::getParameterText (int index)
{
    if(vstInstance)
        return vstInstance->getParameterText(index);
    return String();
}

const String PluginWrapper::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginWrapper::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool PluginWrapper::isInputChannelStereoPair (int index) const
{
    if(vstInstance)
        return vstInstance->isInputChannelStereoPair(index);
    else
        return true;
}

bool PluginWrapper::isOutputChannelStereoPair (int index) const
{
    if(vstInstance)
        return vstInstance->isInputChannelStereoPair(index);
    else
        return true;
}


bool PluginWrapper::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginWrapper::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginWrapper::silenceInProducesSilenceOut() const
{
    return false;
}

double PluginWrapper::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginWrapper::getNumPrograms()
{
    if(vstInstance)
        return vstInstance->getNumPrograms();
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int PluginWrapper::getCurrentProgram()
{
    if(vstInstance)
        return vstInstance->getCurrentProgram();
    return 0;
}

void PluginWrapper::setCurrentProgram (int index) {}

const String PluginWrapper::getProgramName (int index)
{
    if(vstInstance)
        return vstInstance->getProgramName(index);
    return String();
}

void PluginWrapper::changeProgramName (int index, const String& newName)
{
    if(vstInstance)
        return vstInstance->changeProgramName(index, newName);
}

//==============================================================================
void PluginWrapper::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    if(vstInstance)
        vstInstance->prepareToPlay(sampleRate,samplesPerBlock);
}

void PluginWrapper::releaseResources() {}

void PluginWrapper::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    //this processBlock merely processes a loaded VST.
    if(!isBypassed)
    {
        vstInstance->processBlock(buffer,midiMessages);
    }

    if(isMuted)
        buffer.clear();



    rmsLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    rmsRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());


    if(updateCounter==0)
        sendActionMessage("rmsValues "+String(rmsLeft)+" "+String(rmsRight));

    updateCounter++;
    if(updateCounter>5)
        updateCounter=0;
}

//==============================================================================
bool PluginWrapper::hasEditor() const
{
    if(vstInstance)
        return vstInstance->hasEditor();
    else
        return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PluginWrapper::createEditor()
{
    return vstInstance->createEditor();
}

//==============================================================================
void PluginWrapper::getStateInformation (MemoryBlock& destData)
{
    if(vstInstance)
        vstInstance->getStateInformation(destData);
}

void PluginWrapper::setStateInformation (const void* data, int sizeInBytes)
{
    if(vstInstance)
        vstInstance->setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter(AudioPluginInstance* instance)
{
    return new PluginWrapper(instance);
}
