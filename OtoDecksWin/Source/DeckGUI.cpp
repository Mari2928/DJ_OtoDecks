/*
  ==============================================================================

    DeckGUI.cpp
    Created: 4 Aug 2020 12:09:53pm
    Author:  ashigam

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player) : player{ _player }     
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(speedLabel);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(posLabel);

    playButton.addListener(this);
    stopButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);  
}

DeckGUI::~DeckGUI()
{

}

void DeckGUI::resized()
{
    // set bounds for buttons
    double rowH = getHeight() / 6;
    playButton.setBounds(0, 0, getWidth()/2, rowH);
    stopButton.setBounds(getWidth()/2, 0 , getWidth()/2, rowH);

    // set bounds for labels
    volLabel.setBounds(40, rowH, 100, 50);    
    speedLabel.setBounds(getWidth() / 3 + 30, rowH, 100, 50);
    posLabel.setBounds(getWidth() / 3 * 2 + 10, rowH, 100, 50);

    // set bounds for sliders    
    volSlider.setBounds(20, rowH + 30, getWidth()/4, rowH * 3.5);
    speedSlider.setBounds(getWidth() / 3+10, rowH + 30, getWidth() / 4, rowH * 3.5);
    posSlider.setBounds(getWidth() / 3*2, rowH + 30, getWidth() / 4, rowH * 3.5); 
}

/**
*   R1A: Component has custom graphics implemented in a paint function.
*   Set the custom styles and colors for widgets.
*   @param g the graphics to be painted
*/

void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::navy);
    g.fillAll();

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("CustomeDeckGUI", getLocalBounds(),
        juce::Justification::centredBottom, true);   // draw some placeholder text

    // set color for buttons
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));

    // set color for button text
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));

    double rowH = getHeight() / 8;

    // set style for volume label    
    volLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.setColour(juce::Label::textColourId, juce::Colours::lightyellow);

    // set style for speed label    
    speedLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setColour(juce::Label::textColourId, juce::Colours::lightyellow);

    // set style for position label    
    posLabel.setFont(juce::Font(15.0f, juce::Font::bold));
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.setColour(juce::Label::textColourId, juce::Colours::lightyellow);

    // set style for sliders
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

    // set track color for sliders
    volSlider.setColour(juce::Slider::trackColourId, juce::Colour(115, 181, 221));
    speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(115, 181, 221));
    posSlider.setColour(juce::Slider::trackColourId, juce::Colour(115, 181, 221));
}

/**
*   R1B: Component enables the user to control the playback of a deck somehow.
*   implement Button::Listener for PLAY and STOP buttons.
*   @param button the button to be processed
*/

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton) {
        player->setPosition(0);
        player->start();
    }
    if (button == &stopButton) {
        player->stop();
    }    
}

/**
*   R1B: Component enables the user to control the playback of a deck somehow.
*   implement Slider::Listener for sliders.
*   @param slider the slider to be processed
*/

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
        if (slider == &posSlider)
            player->setPositionRelative(slider->getValue());

        if (slider == &speedSlider)
            player->setSpeed(slider->getValue());

        if (slider == &volSlider)
            player->setGain(slider->getValue()); 
}