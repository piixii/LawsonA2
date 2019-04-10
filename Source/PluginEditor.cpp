/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LawsonA2AudioProcessorEditor::LawsonA2AudioProcessorEditor (LawsonA2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
   // setOpaque(true);
    setSize (400, 300);
    
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Wet / Dry Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, false);
    
    mixSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 90, 50);
    mixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mixSlider.setColour(Slider::ColourIds::rotarySliderFillColourId, Colours::greenyellow);
    mixSlider.setRange(0, 100);
    mixSlider.setTextValueSuffix("%");
    mixSlider.addListener(this);
    addAndMakeVisible(mixSlider);
    
    freqSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 40);
    freqSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    freqSlider.setRange(20.0f, 2000.0f);
    freqSlider.setTextValueSuffix("Hz");
    freqSlider.addListener(this);
    addAndMakeVisible(freqSlider);
    
}

LawsonA2AudioProcessorEditor::~LawsonA2AudioProcessorEditor()
{
}

//==============================================================================
void LawsonA2AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::blueviolet);

    g.setColour (Colours::yellowgreen);
   // g.setFont(random.nextFloat()* 24.0f + 12.0f);
    g.drawFittedText ("THIS IS HARD (CLIPPING)", 10, 20, getWidth(), 60, Justification::centred, 1);
    
    
    
    g.setColour(Colours::yellowgreen);//
    //g.setFont(Font("Helvetica", 20.0f, Font::italic));
    //g.drawFittedText("New Text", 10, 10, getWidth(), 20, Justification::left, 1);
    
    g.setColour(Colours::white);
    //g.drawRect(150, 10, 60, 10);
    //g.drawEllipse(60, 160, 40, 40, 5);
    
    mixSlider.setBounds(20, getHeight()-240, 140, 140);
    mixLabel.setBounds(40, getHeight()-160, 80, 160);
    
    freqSlider.setBounds(10, getHeight()-240, 550, 180);
    freqLabel.setBounds(300, getHeight()-220, 80, 190);
}

void LawsonA2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void LawsonA2AudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &mixSlider)
    {
        processor.mixLevel.setTargetValue(mixSlider.getValue());
    } else if (slider== &freqSlider) {
        
    }
}
