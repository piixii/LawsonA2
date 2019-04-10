/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LawsonA2AudioProcessorEditor  : public AudioProcessorEditor, Slider::Listener


{
public:
    LawsonA2AudioProcessorEditor (LawsonA2AudioProcessor&);
    ~LawsonA2AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    Slider mixSlider;
    Slider freqSlider;
    
    Label mixLabel;
    Label freqLabel;
    
    void sliderValueChanged(Slider* slider) override;
    
    LawsonA2AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LawsonA2AudioProcessorEditor)
};
