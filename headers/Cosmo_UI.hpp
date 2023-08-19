#pragma once
#include "Cosmo.h"
#include "Cosmo_Convolution.h"
#include "Cosmo_FileManager.hpp"
#include "Cosmo_IO.h"
#include "Cosmo_Clock.h"



            /**********************************************************************************************EQ_LOGIC**************************************************************************************/


class EQGroup : public Fl_Group
{
public:
    EQGroup(int x, int y, int w, int h) : Fl_Group(x, y, w, h) {}

    void draw() override
    {
        draw_child(*child(0));  // Draw the lower dial first
        draw_child(*child(1));  // Draw the top dial on top
    }
};

//HIGH EQ CONTROL

void setHighFrequencySweep(double freq, int channel)
{
    DialData.High_fc[channel] = freq;
        
    std::cout << "freq: " << DialData.High_fc[channel] << std::endl;
}



void highEqCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);

    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "dial freq @ " << channel << " is :" << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setHighFrequencySweep(frequency, channel);
    }


}

void setHighGain(double gain, int channel)
{
    DialData.High_gain_db[channel] = gain;
    std::cout << "dial gain: " << DialData.High_gain_db[channel] << std::endl;
}



void highGainCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);
    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "dial Gain @ channel: "<< channel<< " is: " << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setHighGain(frequency, channel);
    }


}

//HIGH EQ END

/**************/

//MID EQ

void setMidFrequencySweep(double freq, int channel)
{
    DialData.Mid_fc[channel] = freq;
    std::cout << "MID freq: " << DialData.Mid_fc[channel] << std::endl;
}



void midEqCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);
    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "MID dial freq @ channel: " << channel << " is " << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setMidFrequencySweep(frequency, channel);
    }


}

void setMidGain(double gain, int channel)
{
    DialData.Mid_gain_db[channel] = gain;
    std::cout << "MID dial gain: " << DialData.Mid_gain_db[channel] << std::endl;
}



void midGainCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);
    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "MID dial gain @ channel: " << channel << " is " << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setMidGain(frequency, channel);
    }


}

//END MID EQ

/**************************************/

//LOW EQ

void setLowFrequencySweep(double freq, int channel)
{
    DialData.Low_fc[channel] = freq;
    std::cout << "LOW freq: " << DialData.Low_fc[channel] << std::endl;
}



void LowEqCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);
    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "LOW dial freq @ channel: " << channel << " is " << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setLowFrequencySweep(frequency, channel);
    }


}

void setLowGain(double gain, int channel)
{
    DialData.Low_gain_db[channel] = gain;
    std::cout << "LOW dial gain: " << DialData.Low_gain_db[channel] << std::endl;
}



void lowGainCallback(Fl_Widget* widget, void* userData)
{
    int channel = reinterpret_cast<int>(userData);
    Fl_Dial* dial = dynamic_cast<Fl_Dial*>(widget);
    if (dial)
    {
        double frequency = dial->value();  // Get the dial value
        std::cout << "LOW dial gain @ channel: " << channel << " is " << dial->value() << std::endl;
        // Apply the frequency sweep parameter to the applyHighFilter function
        setLowGain(frequency, channel);
    }


}

//END LOW EQ



void EQDials(int x, int y, int w, int h, int LowDialColor, int HighDialColor, int channel)
{
    void* userData = reinterpret_cast<void*>(channel);

    EQGroup* groupName = new EQGroup(x, y, w, h);
    groupName->begin();

    Fl_Dial* EqLow1 = new Fl_Dial(x, y, w, h);
    Fl_Dial* EqQ = new Fl_Dial(x + 15, y + 15, w - 30, h - 30);

    EqLow1->type(FL_LINE_DIAL);

    if (LowDialColor == 0)
    {
        EqLow1->color(fl_rgb_color(155, 133, 121));


    }

    if (HighDialColor == 1)
    {
        //high
        EqLow1->range(2500, 15000);
        EqLow1->callback(highEqCallback, userData);
        EqQ->color(fl_rgb_color(237, 73, 36));
        EqQ->range(-15, 15);
        EqQ->callback(highGainCallback, userData);

    }
    else if (HighDialColor == 2)
    {
        //mid

        EqQ->color(fl_rgb_color(253, 187, 91));
        EqLow1->range(500, 5000);
        EqLow1->callback(midEqCallback, userData);
        EqQ->range(-15, 15);
        EqQ->callback(midGainCallback, userData);

    }
    else if (HighDialColor == 3)
    {
        //low
        EqQ->color(fl_rgb_color(245,255,118));
        EqLow1->range(50, 1000);
        EqLow1->callback(LowEqCallback, userData);
        EqQ->range(-15, 15);
        EqQ->callback(lowGainCallback, userData);
    }

    EqQ->value(0.55);

    groupName->end();

}




void saturationDialCallback(Fl_Widget* widget, void* userData) 
{
    int channel = reinterpret_cast<int>(userData);

    Fl_Dial* saturationDial = dynamic_cast<Fl_Dial*>(widget);
    DialData.saturationGain[channel] = saturationDial->value();

    std::cout << "dial at: " << channel << " is: " << DialData.saturationGain[channel];

}

void reverbDialCallback(Fl_Widget* widget, void* userData)
{
    Fl_Dial* reverbDial = dynamic_cast<Fl_Dial*>(widget);
    int channel = reinterpret_cast<int>(userData);

    reverb.volume[channel] = reverbDial->value();
    std::cout << "reverbDial value: " << reverb.volume[channel] << std::endl;
}


void Send_Dials(int x, int y, int w, int h, Fl_PNG_Image* SendBG, int channel)
{
    void* Channel = reinterpret_cast<void*>(channel);
  

    Fl_Box* SendBox = new Fl_Box(FL_FLAT_BOX, x - 10, y - 10, 95, 200, 0);
    SendBox->image(SendBG);
    Fl_Dial* Send1_1 = new Fl_Dial(x, y, w, h, 0);
    Fl_Dial* Send2_1 = new Fl_Dial(x, y + 100, w, h, 0);
    Send1_1->color(fl_rgb_color(43, 182, 115));
    Send2_1->color(fl_rgb_color(28,117,188));
    Send1_1->type(FL_LINE_DIAL);
    Send2_1->type(FL_LINE_DIAL);
    Send1_1->bounds(0.0, 10.0);
    Send2_1->bounds(0.0, 1.0);
    Send2_1->value(0);
    Send1_1->callback(saturationDialCallback, Channel);
    Send2_1->callback(reverbDialCallback, Channel);

}


/**********************************************************************************************File_Folder_CREATION**************************************************************************************/


// Function to create the folders
void createFolders(const std::string& path)
{
    std::string rawFolder = path + "/RAW";
    std::string processedFolder = path + "/PROCESSED";

    // Create RAW folder
    if (!fs::create_directory(rawFolder)) {
        std::cerr << "Failed to create RAW folder!" << std::endl;
    }

    // Create PROCESSED folder
    if (!fs::create_directory(processedFolder)) {
        std::cerr << "Failed to create PROCESSED folder!" << std::endl;
    }

    std::cout << "Folders created successfully!" << std::endl;
}

// Function to open an existing project

std::string openProject()
{
    // Open file chooser to select project file
    Fl_File_Chooser fileChooser(".", "*.txt", Fl_File_Chooser::SINGLE, "Select Project File");
    fileChooser.show();

    while (fileChooser.shown()) {
        Fl::wait();
    }

    std::string filePath;
    if (fileChooser.value() != nullptr)
    {
        filePath = fileChooser.value();
        std::cout << "Opening project from file: " << filePath << std::endl;

        // Get the folder path of the selected file
        projectPath = std::filesystem::path(filePath).parent_path().string() + "/";
        std::cout << "Folder path: " << projectPath << std::endl;
    }

    return filePath;
}

//Logic for Opening or Creating a Project
void choiceButtonCallback(Fl_Widget* widget, void* data)
{
    Fl_Choice* choice = static_cast<Fl_Choice*>(data);
    int choiceIndex = choice->value();

    // Close the choice dialog window
    Fl_Window* dialog = static_cast<Fl_Window*>(choice->parent());
    dialog->hide();

    // Creating a new project
    if (choiceIndex == 0)
    {
      
        Fl_Window* newProjectDialog = new Fl_Window(400, 100, "New Project");
        Fl_Input* projectNameInput = new Fl_Input(150, 30, 200, 25, "Project Name:");
        Fl_Button* newProjectButton = new Fl_Button(160, 65, 80, 30, "Create");

        // Callback function for the new project button
        newProjectButton->callback([](Fl_Widget* widget, void* data) {
            Fl_Input* projectNameInput = static_cast<Fl_Input*>(data);
        projectName = projectNameInput->value();

        // Close the new project dialog window
        Fl_Window* dialog = static_cast<Fl_Window*>(projectNameInput->parent());
        dialog->hide();

        // Prompt the user to choose the project folder location
        Fl_File_Chooser fileChooser(".", "*", Fl_File_Chooser::DIRECTORY, "Select Folder Location");
        fileChooser.show();

        while (fileChooser.shown())
        {
            Fl::wait();
        }

        if (fileChooser.value() != nullptr)
        {
            projectPath = fileChooser.value();
            // Creating a new project
            std::cout << projectPath << std::endl;
            createFolders(projectPath);
            createSessionData();
 
        }
        else 
        {
            std::cout << "No folder selected." << std::endl;
            exit(1);
        }
            }, projectNameInput);

        newProjectDialog->end();
        newProjectDialog->show();

        while (newProjectDialog->shown())
        {
            Fl::wait();
        }
    }

    // Opening an existing project - FILEPATH MAY CONFLICT LATER AS THIS RETURNS PATH TO TXT
    else if (choiceIndex == 1)
    {

        filePath = openProject();
        if (!filePath.empty())
        {
            
            if (parseProjectInfo(filePath, project))
            {
                std::cout << "Project Name: " << project.projectName << std::endl;
                std::cout << "Sampling Rate: " << project.samplingRate << std::endl;
                std::cout << "Input Channels: " << project.inputChannels << std::endl;
                std::cout << "Output Channels: " << project.outputChannels << std::endl;

                for (int i = 0; i < 8; ++i)
                {
                    std::cout << "Channel_" << (i + 1) << "[" << project.channelNames[i]
                        << "_" << project.channelLocations[i] << "_"
                        << project.channelLengths[i] << "_"
                        << project.channelStartTimes[i] << "]" << std::endl;
                }

                std::cout << "complete:" << project.channelLocations[0] << "\n" << project.channelLocations[2] << std::endl;
                

            }
            else
            {
                std::cout << "Unable to parse the file." << std::endl;
            }
        }
        else 
        {
            std::cout << "No project file selected." << std::endl;
            exit(1);
        }
    }


}


 /**********************************************************************************************PLAYBACK_RECORD_STATE**************************************************************************************/


void recPlayCallback(Fl_Widget* widget, void* data)
{
    int channel = reinterpret_cast<int>(data);
    /*The dynamic cast is a casting operation that can be used to convert pointers or references of a base class to pointers or references
    of a derived class, or vice versa, at runtime.*/

    Fl_Slider* REC_PLAY = dynamic_cast<Fl_Slider*>(widget);
    


    if (REC_PLAY->value() == 0)
    {
        std::cout << "playback enabled for: " << channel << std::endl;
        project.recPlayBackState[channel] = false;
        std::cout << "PROJECT_SESSION_BOOL STATE: " << project.recPlayBackState[channel] << std::endl;
        std::cout << "CHANNEL LOCATION: " << project.channelLocations[channel] << std::endl;

    }

    else if (REC_PLAY->value() == 1)
    {

        std::cout << "REC enabled for: " << channel << std::endl;
        project.recPlayBackState[channel] = true;
        std::cout << "PROJECT_SESSION_BOOL STATE FOR CHANNEL: " << channel << " is " << project.recPlayBackState[channel] << std::endl;
    }

}


void createRecPlaySlider(int channel, const char* name, int x, int y, int w, int h) 
{
 
    void* channelPtr = reinterpret_cast<void*>(channel);


    Fl_Slider* REC_PLAY = new Fl_Slider(x, y, w, h);
    REC_PLAY->type(FL_VERT_NICE_SLIDER);
    REC_PLAY->label(FL_NORMAL_LABEL, name);
    REC_PLAY->range(0, 1);
    REC_PLAY->step(1.0);
    REC_PLAY->value(0);
   

    REC_PLAY->callback(recPlayCallback, channelPtr);

}


/**********************************************************************************************TRANSPORT**************************************************************************************/

struct TransportButtons
{
    Fl_Button* play;
    Fl_Button* fastForward;
    Fl_Button* rewind;
    Fl_Button* record;

    TransportButtons(Fl_Button* p, Fl_Button* ff, Fl_Button* rw, Fl_Button* rec)
        : play(p), fastForward(ff), rewind(rw), record(rec)
    {
    }

};

void recordButtonCallback(Fl_Widget* button, void* userData)
{
    //std::string recTime = std::to_string(hour) + "_" + std::to_string(minute) + "_" + std::to_string(second);
    
    unsigned long recStartTime = (hour * 3600 + minute * 60 + second) * sRate;
    TransportButtons* transportButtons = static_cast<TransportButtons*>(userData);
    Fl_Button* recButton = static_cast<Fl_Button*>(button);
    rewindOn = false;
    rewindStop = false;
    ffOn = false;
    ffStop = false;
    playBackOn = false;   

    transportButtons->fastForward->color(FL_WHITE);
    transportButtons->rewind->color(FL_WHITE);
    transportButtons->play->color(FL_WHITE);



    // Offload the openStream function to a separate thread
    intialize(err);

    for(int i = 0; i < 8; i++)
    {
        if (project.recPlayBackState[i] == true)
        {
           std::cout << "RECORDING INTIALIZED FOR: " << i << std::endl;
           std::string stopTime = std::to_string(hour) + "_" + std::to_string(minute) + "_" + std::to_string(second);
           project.channelStartTimes[i] = recStartTime;
           dataBuffer* datapass = new dataBuffer;
           datapass->dataChannel = i;

            isRecording = true;
            recButton->label("@circle");
            recButton->color(FL_RED);

            std::thread openStreamThread([&]()
                {
                    openStreamAsync(*datapass, err);
                });

            openStreamThread.detach();  // Detach the thread to let it run independently
            
            project.dataPass[i] = datapass;


        }
        
        else
        {

            std::cout << "PLAYBACK INTIALIZED FOR CHANNEL: " << i << std::endl;
            if (project.channelLocations[i] != " ")
            {
                playBackOn = true;
                std::cout << i << "PROJECT LOCATION: " << project.channelLocations[i] << std::endl;

                std::thread playback(playbackThread, project.channelLocations[i], project.channelStartTimes[i], project.channelLengths[i], i + 1);


                playback.detach();
            }

        }
    
    
    
    }

}



void playButtonCallback(Fl_Widget* widget, void* userData)
{
    globalGif->load("C:\\Users\\alcin\\Desktop\\reels.gif");
    globalGif->speed(1.0);
    globalGif->start();
    ///
    rewindOn = false;
    rewindStop = false;
    ffOn = false;
    ffStop = false;
    ///

    intialize(err);
    playBackOn = true;
    std::cout << "playback clicked" << std::endl;

    TransportButtons* transportButtons = static_cast<TransportButtons*>(userData);
    transportButtons->fastForward->color(FL_WHITE);
    transportButtons->rewind->color(FL_WHITE);
    transportButtons->play->color(FL_YELLOW);

    for (int i = 0; i < 8; i++)
    {
        if (project.recPlayBackState[i] == false && project.channelLocations[i] != " ")
        {
            std::cout << i << "PROJECT LOCATION: " << project.channelLocations[i] << std::endl;
         
            std::thread playback(playbackThread, project.channelLocations[i], project.channelStartTimes[i], project.channelLengths[i], i +1);


            playback.detach();
        }

    }

}



void stopButtonCallback(Fl_Widget* widget, void* data)
{
    bool exitLoop = false;
  //  std::string stopTime = std::to_string(hour) + "_" + std::to_string(minute) + "_" + std::to_string(second);
    unsigned long stopTime = (hour * 3600 + minute * 60 + second) * sRate;
    rewindOn = false;
    rewindStop = false;
    ffOn = false;
    ffStop = false;
    playBackOn = false;
    globalGif->stop();
    TransportButtons* transportButtons = static_cast<TransportButtons*>(data);

    transportButtons->play->color(FL_WHITE);
    transportButtons->rewind->color(FL_WHITE);
    transportButtons->fastForward->color(FL_WHITE);
    transportButtons->record->color(FL_WHITE);

    Fl::redraw();


    if (exitLoop == false && isRecording == true)
    {
        isRecording = false;
        std::cout << "stopping recording" << std::endl;

        for (int i = 0; i < 8; i++)
        {

            if (project.recPlayBackState[i] == true) //need if statement to catch if a user tries to change conditon while rcording 
            {
                
                std::cout << "BOUNCING CHANNEL: " << i << std::endl;
                std::cout << "stopTime: " << stopTime << std::endl;

               
                
                WriteAudio(*(project.dataPass[i]), i );
                convolution(i, 1);
                project.channelLengths[i] = stopTime;
                delete project.dataPass[i];

            }
        }
        createDocument(filePath, project);
        exitLoop = true;
        terminate(err);


    }
    else if (isRecording == false)
    {
        std::cout << "Stoping Transport" << std::endl;

    }

}


void rewindButtonCallback(Fl_Widget* widget, void* data)
{
    globalGif->load("C:\\Users\\alcin\\Desktop\\reelsReverse.gif");
    globalGif->speed(4.0);
    globalGif->start();
    
    ffOn = false;
    ffStop = false;

    TransportButtons* transportButtons = static_cast<TransportButtons*>(data);
    Fl_Button* RewindButton = dynamic_cast<Fl_Button*>(widget);


    if (!isRecording || !playBackOn)
    {



        if (rewindOn == true && rewindStop == false)
        {
            rewindOn = false;
            RewindButton->color(FL_WHITE);

        }
        else if (rewindOn == false && rewindStop == false)
        {
            rewindOn = true;
            RewindButton->color(FL_BLUE);
            transportButtons->play->color(FL_WHITE);
            transportButtons->fastForward->color(FL_WHITE);


        }
        else if (rewindStop == true)
        {

            RewindButton->color(FL_WHITE);
            rewindStop = false;

        }
    }
    else
    {

        std::cout << "Recording in Progress." << std::endl;

    }

}

void fastForwardButtonCallback(Fl_Widget* widget, void* data)
{
    globalGif->load("C:\\Users\\alcin\\Desktop\\reels.gif");
    globalGif->speed(4.0);
    globalGif->start();
    rewindOn = false;
    rewindStop = false;

    TransportButtons* transportButtons = static_cast<TransportButtons*>(data);
    Fl_Button* ffButton = dynamic_cast<Fl_Button*>(widget);
    if (!isRecording || rewindOn == true )//?
    {


        if (ffOn)
        {
            ffOn = false;
            ffButton->color(FL_WHITE);


        }
        else if (ffOn == false)
        {
            ffOn = true;
            ffButton->color(FL_RED);
            transportButtons->play->color(FL_WHITE);
            transportButtons->rewind->color(FL_WHITE);

        }
    }
    else
    {
        std::cout << "recording in progress" << std::endl;
    }
}

void loadButtonCallback(Fl_Widget* widget, void*)
{

    Fl_File_Chooser chooser(".", "*.{wav,txt}", Fl_File_Chooser::SINGLE, "Select a file");
    chooser.show();

    while (chooser.shown()) {
        Fl::wait();
    }

    if (chooser.value() == NULL) {
        // User canceled the selection
        return;
    }

    std::string filename = chooser.value();
    std::string fileExtension = filename.substr(filename.find_last_of(".") + 1);

    if (fileExtension == "wav")
    {
        selectedWavFile = filename;

        // Cue the user to select a number between 1 and 8
        const char* numStr = fl_input("Enter a number between 1 and 8:");
        if (numStr != NULL) {
            selectedNumber = atoi(numStr);
            project.channelLocations[selectedNumber -1] = filename;
            std::cout << project.channelLocations[selectedNumber -1] << " written to channel location " << numStr << std::endl;
            std::cout << "filename: " << filename << std::endl;
           convolution(selectedNumber - 1, 0); 
           createDocument(filePath, project);
        }
    }

    else if (fileExtension == "txt")
    {
        std::cout << "txt choosen" << std::endl;
        parseProjectInfo(filename, project);
        std::cout << project.channelLocations[0] << std::endl;


    }

    else { std::cout << "Invalid File selected" << std::endl; }

}

void saveButtonCallback(Fl_Widget* widget, void*)
{




}

void sliderVolumeCallback(Fl_Widget* widget, void* channelNumber)
{
    std::cout << "VOLUME KNOB SELECTED: " << (int)channelNumber << std::endl;
    Fl_Slider* volumeSlider = dynamic_cast<Fl_Slider*>(widget);
    project.sliderVolume[(int)channelNumber] = volumeSlider->value();
    std::cout << "Project Value = " << project.sliderVolume[(int)channelNumber] << std::endl;
    Fl::redraw();


}

void createVolumeSlider(int x, int y, int w, int h, int channelNumber)
{
    Fl_Slider* volumeSlider = new Fl_Slider(x, y, w, h);
    volumeSlider->type(FL_VERT_NICE_SLIDER);
    volumeSlider->bounds(1.0, 0);
    volumeSlider->slider_size(0.05);
    volumeSlider->box(FL_THIN_UP_FRAME);
    //volumeSlider->slider(FL_NO_BOX);

    volumeSlider->callback(sliderVolumeCallback, (void*) channelNumber);

}


/**********************************************************************************************INTIALIZERS**************************************************************************************/



void sessionSetup()
{
    intialize(err);

    StreamSetup(err);

    // Prompt the user to choose between creating a new project or opening an existing one
    Fl_Window choiceDialog(300, 120, "Choose Project");
    Fl_Choice projectChoice(120, 30, 150, 25, "Choose Project:");
    projectChoice.add("Create New Project");
    projectChoice.add("Open Existing Project");
    Fl_Button choiceButton(110, 80, 80, 30, "Choose");
    choiceDialog.end();

    // Callback function for the choice button
    choiceButton.callback(choiceButtonCallback, &projectChoice);

    choiceDialog.show();
    while (choiceDialog.shown())
    {
        Fl::wait();
    }

  

}

Fl_Double_Window* mainWindow()
{
    //Window and Box Drop
    Fl_Double_Window* window = new Fl_Double_Window(1800, 1000, "Cosmo38");
    Fl_PNG_Image* windowImage = new Fl_PNG_Image("C:\\Users\\alcin\\Desktop\\Background.png");
    Fl_Box* imagebox = new Fl_Box(0, 0, 1800, 1000);
    imagebox->image(windowImage);


    //Clock Counter
    TimeBox* timeBox = new TimeBox(1290, 460, 400, 100);
    timeBox->box(FL_UP_BOX);
    timeBox->labelsize(24);
    timeBox->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    timeBox->color(FL_BLACK);
    timeBox->labelcolor(FL_DARK_GREEN);
    timeBox->labelfont(FL_SCREEN_BOLD);

    Fl_Box* logo = new Fl_Box(1460, 5, 100, 80);




    //Transport Display
    Fl_Button* Play = new Fl_Button(1390, 820, 100, 100);
    Fl_Button* Rewind = new Fl_Button(1290, 820, 100, 100);
    Fl_Button* Stop = new Fl_Button(1490, 820, 100, 100);
    Fl_Button* FastForward = new Fl_Button(1590, 820, 100, 100);
    Fl_Button* recordButton = new Fl_Button(1390, 720, 200, 100, "@circle");
    Fl_Button* loadButton = new Fl_Button(1290, 720, 100, 100);
    Fl_Button* saveButton = new Fl_Button(1590, 720, 100, 100);
    TransportButtons* transportButtons = new TransportButtons(Play, FastForward, Rewind, recordButton);

    loadButton->label("@fileopen");
    loadButton->callback(loadButtonCallback);
    loadButton->labelsize(18);

    saveButton->label("@filesave");
    saveButton->callback(saveButtonCallback);
    saveButton->labelsize(18);

    Play->label("@>");
    Play->labelsize(22);
    Play->callback(playButtonCallback,  transportButtons);
  
    Rewind->label("@<<");
    Rewind->callback(rewindButtonCallback, transportButtons);
    Rewind->labelsize(18);

    FastForward->label("@>>");
    FastForward->callback(fastForwardButtonCallback,  transportButtons);
    FastForward->labelsize(18);

    recordButton->callback(recordButtonCallback,  transportButtons);
    recordButton->labelsize(16);

    Stop->label("@square");
    Stop->callback(stopButtonCallback, transportButtons);
    Stop->labelsize(16);

    // Record or Playback Sliders
    createRecPlaySlider(0, "1", 1240, 580, 35, 100);
    createRecPlaySlider(1, "2", 1306, 580, 35, 100);
    createRecPlaySlider(2, "3", 1372, 580, 35, 100);
    createRecPlaySlider(3, "4", 1438, 580, 35, 100);
    createRecPlaySlider(4, "5", 1504, 580, 35, 100);
    createRecPlaySlider(5, "6", 1570, 580, 35, 100);
    createRecPlaySlider(6, "7", 1635, 580, 35, 100);
    createRecPlaySlider(7, "8", 1700, 580, 35, 100);

    // VU Meters Image
    Fl_PNG_Image* pngImage = new Fl_PNG_Image("C:\\Users\\alcin\\Desktop\\VuMeter.png");

    // VU Meters
    Fl_Box* channel1 = new Fl_Box(0, 0, 200,  130, 0);
    Fl_Box* channel2 = new Fl_Box(0, 0, 500,  130, 0);
    Fl_Box* channel3 = new Fl_Box(0, 0, 800,  130, 0);
    Fl_Box* channel4 = new Fl_Box(0, 0, 1100, 130, 0);
    Fl_Box* channel5 = new Fl_Box(0, 0, 1400, 130, 0);
    Fl_Box* channel6 = new Fl_Box(0, 0, 1700, 130, 0);
    Fl_Box* channel7 = new Fl_Box(0, 0, 2000, 130, 0);
    Fl_Box* channel8 = new Fl_Box(0, 0, 2300, 130, 0);

    channel1->image(pngImage);
    channel2->image(pngImage);
    channel3->image(pngImage);
    channel4->image(pngImage);
    channel5->image(pngImage);
    channel6->image(pngImage);
    channel7->image(pngImage);
    channel8->image(pngImage);

   //Reel 2 Reel Animation; research CPU usage.. caching 
    globalGif = new Fl_Anim_GIF(1240, 140, 498, 309, "C:\\Users\\alcin\\Desktop\\Reels.gif");
    globalGif->stop();

    //Volume Sliders
    createVolumeSlider(30, 680, 136, 300, 1);
    createVolumeSlider(180, 680, 136, 300, 2);
    createVolumeSlider(330, 680, 136, 300, 3);
    createVolumeSlider(480, 680, 136, 300, 4);
    createVolumeSlider(630, 680, 136, 300, 5);
    createVolumeSlider(780, 680, 136, 300, 6);
    createVolumeSlider(930, 680, 136, 300, 7);
    createVolumeSlider(1080,680, 136, 300, 8);

    //EQ DIALS

    //Column1
    Fl_PNG_Image* columnTest = new Fl_PNG_Image("C:\\Users\\alcin\\Desktop\\dial.png");
    Fl_Box* Column1 = new Fl_Box(FL_FLAT_BOX, 52, 105, 95, 325, 0);
    Column1->image(columnTest);
    Column1->type(FL_FLAT_BOX);
    Column1->color(fl_rgb_color(233, 234, 230));;
    EQDials(57, 125, 85, 85, 0, 1, 0);
    EQDials(57, 222, 85, 85, 0, 2, 0);
    EQDials(57, 320, 85, 85, 0, 3, 0);

    //Column2 
    Fl_Box* Column2 = new Fl_Box(FL_FLAT_BOX, 202, 105, 95, 325, 0);
    Column2->image(columnTest);
    EQDials(207, 125, 85, 85, 0, 1, 1);
    EQDials(207, 222, 85, 85, 0, 2, 1);
    EQDials(207, 320, 85, 85, 0, 3, 1);

    //Column3

    Fl_Box* Column3 = new Fl_Box(FL_FLAT_BOX, 352, 105, 95, 325, 0);
    Column3->image(columnTest);
    EQDials(357, 125, 85, 85, 0, 1 ,2);
    EQDials(357, 222, 85, 85, 0, 2, 2);
    EQDials(357, 320, 85, 85, 0, 3 ,2);

    //Column4
    Fl_Box* Column4 = new Fl_Box(FL_FLAT_BOX, 502, 105, 95, 325, 0);
    Column4->image(columnTest);
    EQDials(507, 125, 85, 85, 0, 1, 3);
    EQDials(507, 222, 85, 85, 0, 2, 3);
    EQDials(507, 320, 85, 85, 0, 3, 3);

    //Column5
    Fl_Box* Column5 = new Fl_Box(FL_FLAT_BOX, 652, 105, 95, 325, 0);
    Column5->image(columnTest);
    EQDials(657,125, 85, 85, 0, 1, 4);
    EQDials(657,222, 85, 85, 0, 2, 4);
    EQDials(657,320, 85, 85, 0, 3, 4);

    //Column6
    Fl_Box* Column6 = new Fl_Box(FL_FLAT_BOX, 802, 105, 95, 325, 0);
    Column6->image(columnTest);
    EQDials(807,125, 85, 85, 0, 1 ,5);
    EQDials(807,222, 85, 85, 0, 2, 5);
    EQDials(807,320, 85, 85, 0, 3, 5);

    //Column7
    Fl_Box* Column7 = new Fl_Box(FL_FLAT_BOX, 951, 105, 95, 325, 0);
    Column7->image(columnTest);
    EQDials(957, 125, 85, 85, 0, 1, 6);
    EQDials(957, 222, 85, 85, 0, 2, 6);
    EQDials(957, 320, 85, 85, 0, 3, 6);

    //Column8
    Fl_Box* Column8 = new Fl_Box(FL_FLAT_BOX, 1102, 105, 95, 325, 0);
    Column8->image(columnTest);
    EQDials(1107, 125, 85, 85, 0, 1, 7);
    EQDials(1107, 222, 85, 85, 0, 2, 7);
    EQDials(1107, 320, 85, 85, 0, 3, 7);

    Fl_PNG_Image* SendBG = new Fl_PNG_Image("C:\\Users\\alcin\\Desktop\\eqdial.png");

    //SENDS 1 AND 2 
    Send_Dials(60,  465, 75, 75, SendBG, 0);
    Send_Dials(210, 465, 75, 75, SendBG, 1);
    Send_Dials(360, 465, 75, 75, SendBG, 2);
    Send_Dials(510, 465, 75, 75, SendBG, 3);
    Send_Dials(660, 465, 75, 75, SendBG, 4);
    Send_Dials(810, 465, 75, 75, SendBG,5);
    Send_Dials(960, 465, 75, 75, SendBG,6);
    Send_Dials(1110,465, 75, 75, SendBG, 7);



    return window;

}