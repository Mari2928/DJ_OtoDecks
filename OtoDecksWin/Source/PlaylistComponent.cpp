/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 26 Aug 2020 5:25:27am
    Author:  ashigam

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <algorithm>

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, 
                            juce::AudioFormatManager& formatManagerToUse, 
                            juce::AudioThumbnailCache& cacheToUse,
                            juce::AudioFormatManager& formatManagerToUse2, 
                            juce::AudioThumbnailCache& cacheToUse2 )
                            : player1{ _player1 }, player2{ _player2 }, 
                            waveformDisplay(formatManagerToUse, cacheToUse), 
                            waveformDisplay2(formatManagerToUse2, cacheToUse2)
    
{
    // R2E: restore library
    restoreLibrary();

    // create columns for the library
    tableComponent.getHeader().addColumn("Track Title", 0, 200);    
    tableComponent.getHeader().addColumn("Length", 3, 200);
    tableComponent.getHeader().addColumn("Load to Deck1", 1, 200);
    tableComponent.getHeader().addColumn("Load to Deck2", 2, 200);
    
    // add table component 
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    // add ADD button 
    addAndMakeVisible(addButton);
    addButton.addListener(this); 

    // add searchBox text editor
    addAndMakeVisible(searchBox);
    searchBox.addListener(this);

    // add waveforms
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(waveformDisplay2);
    startTimer(500);
}

PlaylistComponent::~PlaylistComponent()
{
    
}

/**
*   Set the location (counds) for the add button, the search box, and the table component.
*/
void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8;

    // set bounds for widgets
    waveformDisplay.setBounds(0, 0, getWidth() / 2, rowH * 2.5);
    waveformDisplay2.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH * 2.5);
    addButton.setBounds(0, rowH * 2.5, getWidth()/2, rowH * 0.7);
    searchBox.setBounds(getWidth() / 2, rowH * 2.5, getWidth() / 2, rowH * 0.7);
    tableComponent.setBounds(0, rowH * 3.2, getWidth(), rowH * 4.8);
    
}

/**
*   Get the number of rows in the table component.
*   @return the number of rows.
*/
int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

/**
*   Paint the row background. 
*   @param g the graphics to be painted
*   @param rowNumber the number of the row
*   @param width the number of width of the row
*   @param height the number of height of the row
*   @param rowIsSelected true if the row is selected and false otherwise
*/
void PlaylistComponent::paintRowBackground(juce::Graphics& g, 
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected )
{
    if (rowIsSelected) {
        g.fillAll(juce::Colour(115, 181, 221)); // use same color with waveform
    }
    else {
        g.fillAll(juce::Colours::white);
    }
}

/**
*   R1A: Component has custom graphics implemented in a paint function.
*   Set the custom styles and colors for widgets.
*   @param g the graphics to be painted
*/

void PlaylistComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);   // draw some placeholder text

    // set color for the add button and its text
    addButton.setColour(juce::TextButton::buttonColourId, juce::Colour(255, 53, 90));
    addButton.setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));

    // set default text to show on a search box as well as set color
    searchBox.setTextToShowWhenEmpty("Search...", juce::Colours::darkgrey);
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colour(255, 255, 255));
    searchBox.setColour(juce::TextEditor::textColourId, juce::Colours::darkgrey);
}

/**
*   R1B: Component enables the user to control the playback of a deck somehow.
*   R2D: Component allows the user to load files from the library into a deck.
*   If LOAD1 or LOAD2 button is clicked, load a track to a subjected deck and to a waveform.
*   @param buttonId the button id number
*/

void PlaylistComponent::loadFromLibrary(int buttonId)
{
    if (buttonId % 2 == 0) {  // even ID: load to player 1
        player1->loadURL(juce::URL{ trackFiles[buttonId / 2] });
        waveformDisplay.loadURL(juce::URL{ trackFiles[buttonId / 2] });
    }
    else if (buttonId % 2 == 1) {  // odd ID: load to player 2
        player2->loadURL(juce::URL{ trackFiles[buttonId / 2] });
        waveformDisplay2.loadURL(juce::URL{ trackFiles[buttonId / 2] });
    }
}

/**
*   R1B: Component enables the user to control the playback of a deck somehow.
*   Set the relative position for waveform so the play head can move in a real time.
*   The user can trigger a waveform by clicking on a LOAD1/LOAD2 button which calls this function.
*/

void PlaylistComponent::timerCallback()
{
    waveformDisplay.setPositionRelative(player1->getPositionRelative());
    waveformDisplay2.setPositionRelative(player2->getPositionRelative());
}

/**
*   R2A: Component allows the user to add files to their library.
*   R2D: Component allows the user to load files from the library into a deck.
*   Perform operations depending on which button is clicked.
*   @param button the reference of the button.
*/

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // ADD button is clicked: add music to the library as well as save it to Tracks folder
    if (button == &addButton) {
        addFileToLibrary();
    }

    // LOAD1 or LOAD2 button is clicked: load files from the library    
    else {
        int buttonId = std::stoi(button->getComponentID().toStdString());
        loadFromLibrary(buttonId);
    }
}

/**
*   R2A: Component allows the user to add files to their library.
*/

void PlaylistComponent::addFileToLibrary()
{
    // reset the library first because the vector value maybe changed by search operation
    resetAll();
    restoreLibrary();

    juce::FileChooser chooser{ "Select a file..." };

    if (chooser.browseForFileToOpen()) {

        // create Tracks folder if not exist
        juce::String MyFolderPath(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks");
        juce::File myFolder(MyFolderPath);
        if (myFolder.isDirectory() == false)
        {
            myFolder.createDirectory();
        }

        // get the title of the selected file and make a space for the file in Tracks folder
        juce::String title = chooser.getResult().getFileName();
        juce::File audioFileCopy(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks/" + title);

        // add if title is not in the library
        if (!(std::find(trackTitles.begin(), trackTitles.end(), title) != trackTitles.end())) {
            if (chooser.getResult().copyFileTo(audioFileCopy)) {
                push_backMetadata(audioFileCopy, title);
            }
        }
        // update the library 
        tableComponent.updateContent();
        tableComponent.repaint();
        return;
    }
}

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
void PlaylistComponent::paintCell(juce::Graphics& g,
                            int rowNumber, 
                            int columnId,
                            int width, 
                            int height, 
                            bool rowIsSelected)
{
    // display track titles (file names)
    if (columnId == 0) {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height,
            juce::Justification::centredLeft, true);
    }
    // display track lengths
    else if (columnId == 3) {
        g.drawText(trackLengths[rowNumber], 2, 0, width - 4, height,
            juce::Justification::centredLeft, true);
    }
}

/**
*   R2B: Component parses and displays metadata such as file name and song length.
*   Parse the track file's metadata and push it back to vectors to be displayed by paintCell().
*   @param trackFile the track file in which the metadata to be pushed back
*/

void PlaylistComponent::push_backMetadata(juce::File trackFile, juce::String title)
{
    trackFiles.push_back(trackFile);
    trackTitles.push_back(title);
    trackLengths.push_back(getTrackLength(trackFile));
}

/**
*   R2B: Component parses and displays metadata such as file name and song length.
*   Create the reader for the track file, calculate the song length, and convert it to time.
*   @param trackFile the track file to be read
*   @return the string of time
*/

juce::String PlaylistComponent::getTrackLength(juce::File trackFile)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();   // add basic music file formats

    // create a format reader and calculate the song length
    juce::AudioFormatReader* formatReader = formatManager.createReaderFor(trackFile);
    int totalLength = formatReader->lengthInSamples / formatReader->sampleRate;

    // convert the length to time
    return std::to_string(totalLength / 60) + ":" + std::to_string(totalLength % 60);
}

/**
*   R2C: Component allows user to search for files.
*   Implementation of pure virtual function of TextEditor class to satisfy this requirement.
*   Display the matched files in the library when return key is pressed after user inputs the keyword.
*   @param the reference of the text editor value
*/

void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor&)
{
    // empty titles and files
    resetAll();

    juce::String titleKeyword = searchBox.getText();
    juce::String MyFolderPath(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks");
    juce::File myFolder(MyFolderPath);

    // go through Tracks folder and display only if the title contains the keyword
    if (myFolder.isDirectory())
    {
        juce::DirectoryIterator iter(juce::File(MyFolderPath), true);

        while (iter.next()) // iterate over the Tracks folder
        {
            juce::File theFileItFound(iter.getFile());
            juce::String title = theFileItFound.getFileName();

            if (title.contains(titleKeyword)) {
                push_backMetadata(theFileItFound, title);
            }
        }
        // update the library 
        tableComponent.updateContent();
        tableComponent.repaint();
    }
}

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

juce::Component* PlaylistComponent::refreshComponentForCell(
                            int rowNumber, 
                            int columnId,
                            bool rowIsSelected,
                            Component* existingComponentToUpdate )
{
    // create a LOAD1 button
    if (columnId == 1) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("LOAD1");
            juce::String id{std::to_string(rowNumber * 2)};  // even number of ID
            btn->setComponentID(id);
            
            btn->addListener(this);
            existingComponentToUpdate = btn;

            btn->setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
            btn->setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
        }
    }
    // create a LOAD2 button
    else if (columnId == 2) {
        if (existingComponentToUpdate == nullptr) {
            juce::TextButton* btn = new juce::TextButton("LOAD2");
            juce::String id{ std::to_string(rowNumber * 2 + 1) }; // odd number of ID
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;

            btn->setColour(juce::TextButton::buttonColourId, juce::Colour(255, 219, 255));
            btn->setColour(juce::TextButton::textColourOffId, juce::Colour(34, 53, 70));
        }
    }
    return existingComponentToUpdate;
}

/**
*   R2E: The music library persists so that it is restored
*   when the user exits then restarts the application.
*   If there is any stored music (Tracks folder exists), restore it to library.
*/

void PlaylistComponent::restoreLibrary()
{    
    juce::String sMyFolderPath(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks");
    juce::File myFolder(sMyFolderPath);
    if (myFolder.isDirectory()) // Tracks folder exists
    {
        juce::DirectoryIterator iter(juce::File(sMyFolderPath), true);

        while (iter.next()) // iterate over the Tracks folder 
        {
            juce::File theFileItFound(iter.getFile());
            juce::String title = theFileItFound.getFileName();
            push_backMetadata(theFileItFound, title);
        }
        // update the library 
        tableComponent.updateContent();
        tableComponent.repaint();
    }
}

/** 
*   Helper: Reset all vectors (trackFiles, trackNames, trackLengths). 
*/

void PlaylistComponent::resetAll()
{
    trackTitles.clear();
    trackFiles.clear();
    trackLengths.clear();
}
