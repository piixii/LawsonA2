/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LawsonA2AudioProcessor::LawsonA2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 0.0f;
}

LawsonA2AudioProcessor::~LawsonA2AudioProcessor()
{
}

//==============================================================================
const String LawsonA2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LawsonA2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LawsonA2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LawsonA2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LawsonA2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LawsonA2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LawsonA2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void LawsonA2AudioProcessor::setCurrentProgram (int index)
{
}

const String LawsonA2AudioProcessor::getProgramName (int index)
{
    return {};
}

void LawsonA2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void LawsonA2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    sinFreq = 20.0f;
    updateangleDelta();
    
    mixLevel.reset(sampleRate, 0.05f);
    mixLevel.setTargetValue(0.25f);
    mixLevel = (0.15f);
    
    gain.setGainDecibels(12.0f);
    
    
    String message;
    message << "Preparing to play..." << newLine;
  //  message << "my sample rate is..."
}

void LawsonA2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LawsonA2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void LawsonA2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
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
    
    //set up another copy of the input buffer so we can keep the original
    AudioBuffer<float> wetBuffer(totalNumInputChannels, buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer(channel);
        
        //we are getting an audio block size of 512

        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            //generate a set of random values to modulate input amplitude
            // wetData[sample] = wetData[sample] * modulator; //scaling first and offsetting second
            
            //            ATTN JON: sounds best with a keys loop or a drum loop
            
            //calculate value to put into buffer
            auto currentSinSample = (float) std::sin(currentAngle);
            currentAngle += angleDelta;
            wetData[sample] = wetData[sample] * 0.5f;
            
            auto shapedSample = jlimit((float) -0.8, (float) 0.8, wetData[sample]);
            wetData[sample] = shapedSample;
            
//adding the original dry signal to processed signal into our output buffer (aka input buffer)
            channelData[sample] = channelData[sample] * 0.3f + wetData[sample] * 0.7f;
//            hard clipping
            channelData[sample] = jlimit ((float) -0.1, (float) 0.1, wetData[sample]);
            channelData[sample] = channelData[sample] * (0.25f - mixLevel.getNextValue()) + wetData[sample] * mixLevel.getNextValue();
        }
    }
    dsp::AudioBlock<float> output(buffer);
    gain.process(dsp::ProcessContextReplacing<float> (output));
    
}

//==============================================================================
bool LawsonA2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* LawsonA2AudioProcessor::createEditor()
{
    return new LawsonA2AudioProcessorEditor (*this);
}

//==============================================================================
void LawsonA2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LawsonA2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}
//User functions- new functions that i created

void LawsonA2AudioProcessor::updateangleDelta()
{
    //calculate the number of cycles that will need to be completed for each output sample
    auto cyclesPerSample = sinFreq / currentSampleRate;
    //multiply by the length of one whole sine wave cycle, for scaling
    angleDelta = cyclesPerSample * MathConstants<float>::twoPi;
    
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LawsonA2AudioProcessor();
}
