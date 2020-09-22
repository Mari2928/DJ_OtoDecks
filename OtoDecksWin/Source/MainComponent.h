#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent

{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

    /** test */
    //void sliderDragStarted(juce::Slider*) override;
    std::string message = "";

private:
    //==============================================================================
    bool playing;
    double gain;

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{ &player1 };

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{ &player2 };

    juce::MixerAudioSource mixerSource;

    // added two pairs of formatManager and thumbCache to display waveforms in the playlist
    PlaylistComponent playlistComponent{ &player1, &player2,  formatManager, thumbCache,  formatManager, thumbCache };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
