/*
  ==============================================================================

    PlaylistComponent.h
    Created: 26 Aug 2020 5:25:27am
    Author:  ashigam

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener,
                           public juce::Timer
    
{
public:
    PlaylistComponent(DJAudioPlayer* player1, DJAudioPlayer* player2,
                            juce::AudioFormatManager& formatManagerToUse, 
                            juce::AudioThumbnailCache& cacheToUse,
                            juce::AudioFormatManager& formatManagerToUse2, 
                            juce::AudioThumbnailCache& cacheToUse2 );                        

    ~PlaylistComponent() override;
    
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(juce::Graphics&, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected )override;

    /**
    *   R1A: Component has custom graphics implemented in a paint function.
    *   Set the custom styles and colors for widgets.
    *   @param g the graphics to be painted
    */

    void paint(juce::Graphics&) override;

    /**
    *   R1B: Component enables the user to control the playback of a deck somehow.
    *   R2D: Component allows the user to load files from the library into a deck.
    *   If LOAD1 or LOAD2 button is clicked, load a track to a subjected deck and to a waveform.
    *   @param buttonId the button id number
    */

    void loadFromLibrary(int ID);

    /**
    *   R1B: Component enables the user to control the playback of a deck somehow.
    *   Set the relative position for waveform so the play head can move in a real time.
    *   The user can trigger a waveform by clicking on a LOAD1/LOAD2 button which calls this function.
    */

    void timerCallback() override;

    /**
    *   R2A: Component allows the user to add files to their library.
    *   R2D: Component allows the user to load files from the library into a deck.
    *   Perform operations depending on which button is clicked.
    *   @param button the reference of the button.
    */

    void buttonClicked(juce::Button* button)override;
    
    /**
    *   R2A: Component allows the user to add files to their library.
    */

    void addFileToLibrary();    

    /**
    *   R2B: Component parses and displays metadata such as file name and song length.
    *   Implementation of the pure virtual function of TableListBoxModel class to satisfy this requirement.
    *   Display the file name and the song length by painting the cell.
    *   @param g the graphics to be painted
    *   @param rowNumber the number of the row
    *   @param columnId the id number of the column
    *   @param width the number of width of the cell
    *   @param height the number of height of the cell
    *   @param rowIsSelected true if the row is selected and false otherwise
    */

    void paintCell(juce::Graphics&, 
                            int rowNumber, 
                            int columnId,
                            int width, 
                            int height,
                            bool rowIsSelected )override;

    /**
    *   R2B: Component parses and displays metadata such as file name and song length.
    *   Parse the track file's metadata and push it back to vectors to be displayed by paintCell().
    *   @param trackFile the track file in which the metadata to be pushed back
    */

    void push_backMetadata(juce::File trackFile, juce::String title);

    /**
    *   R2B: Component parses and displays metadata such as file name and song length.
    *   Create the reader for the track file, calculate the song length, and convert it to time.
    *   @param trackFile the track file to be read
    *   @return the string of time
    */

    juce::String getTrackLength(juce::File trackFile); 

    /**
    *   R2C: Component allows user to search for files.
    *   Implementation of pure virtual function of TextEditor class to satisfy this requirement.
    *   Display the matched files in the library when return key is pressed after user inputs the keyword.
    *   @param the reference of the text editor value
    */

    void textEditorReturnKeyPressed(juce::TextEditor&)override;

    /**
    *   R2D: Component allows the user to load files from the library into a deck.
    *   Refresh component for cell creating the LOAD1 and LOAD2 buttons for each file.
    *   To distinguish the two buttons, even number is assigned to LOAD1 and odd number for LOAD2.
    *
    *   @param rowNumber the number of the row
    *   @param columnId the id number of the column
    *   @param rowIsSelected true if the row is selected and false otherwise
    *   @param existingComponentToUpdate the component to be updated
    *   @return the updated existing Component
    */

    Component* refreshComponentForCell(
                            int rowNumber, 
                            int columnId,
                            bool isRowSelected,
                            Component* existingComponentToUpdate )override;

    /**
    *   R2E: The music library persists so that it is restored
    *   when the user exits then restarts the application.
    *   If there is any stored music (Tracks folder exists), restore it to library.
    */

    void restoreLibrary();    

    /**
    *   Helper: Reset all vectors (trackFiles, trackNames, trackLengths).
    */

    void resetAll();


private:   

    WaveformDisplay waveformDisplay;
    WaveformDisplay waveformDisplay2;
    
    juce::TextButton addButton{ "ADD" };

    juce::TableListBox tableComponent;
    std::vector<juce::String> trackTitles;
    std::vector<juce::File> trackFiles;
    std::vector<juce::String> trackLengths;

    // R2D: to load a track from the library
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;

    // R2C: to search for files by keyword 
    juce::TextEditor searchBox;    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
