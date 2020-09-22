/*
  ==============================================================================

    DeckGUI.h
    Created: 4 Aug 2020 12:09:53pm
    Author:  ashigam

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener               
{
public:
    DeckGUI(DJAudioPlayer* _player);

    ~DeckGUI() override;
    
    void resized() override;

    /**
    *   R1A: Component has custom graphics implemented in a paint function.
    *   Set the custom styles and colors for widgets.
    *   @param g the graphics to be painted
    */

    void paint(juce::Graphics& g) override;

    /** 
    *   R1B: Component enables the user to control the playback of a deck somehow.
    *   implement Button::Listener for PLAY and STOP buttons.
    *   @param button the button to be processed
    */

    void buttonClicked(juce::Button* button) override;

    /**
    *   R1B: Component enables the user to control the playback of a deck somehow.
    *   implement Slider::Listener for sliders.
    *   @param slider the slider to be processed
    */

    void sliderValueChanged(juce::Slider* slider) override;


private:
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };

    DJAudioPlayer* player;

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Label speedLabel;
    juce::Label volLabel;
    juce::Label posLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
