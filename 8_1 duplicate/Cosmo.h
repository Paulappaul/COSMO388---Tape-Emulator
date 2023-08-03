#pragma once
//STD
#include <iostream>
#include <conio.h>
#include <random>
#include <vector>
#include <cmath>
#include <thread>
//#include "dirent.h" // need? 
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>


#include <future>
//AUDIO STREAM AND WRITE
#include <audioFile.h>
#include "portaudio.h"

//FFT
#include <FFTConvolver.h>
#include <FFTConvolver.cpp>
#include <AudioFFT.h>
#include <AudioFFT.cpp>
#include <Utilities.h>
#include <Utilities.cpp>

//FLTK
#include "Fl/Fl.H"
#include "Fl/Fl_Window.H"
#include "Fl/Fl_Button.H"
#include "Fl/Fl_Slider.H"
#include "Fl/Fl_Dial.H"
#include "Fl/Fl_Box.H"
#include "FL/Fl_JPEG_Image.H"
#include "FL/Fl_PNG_Image.H"
#include "FL/Fl_Button.H"
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include "FL/Fl_Counter.H"
#include <Fl_Anim_GIF.H>
#include <Fl_Anim_GIF.CPP>


#include <string>




/*NAMESPACE*/
namespace fs = std::filesystem;
using namespace fftconvolver;

/*PortAudio Variables*/
PaError err;
PaStreamParameters outputParameters;
PaStreamParameters inputParameters;

//File_Manager
std::string filename;
std::string filePath = " ";


/*GLOBAL VARIABLES*/
std::string projectName = "";
std::string projectPath = "";
Fl_Anim_GIF* globalGif;


int ChannelNum = 0;
double sRate = 44100;
const double pi = 3.14159265358979323846;

//Cosmo_TIME
int hour = 0;
int minute = 0;
int second = 0;

//Transport
//do we need stops and ons? 
bool rewindOn = false;
bool rewindStop = false; 
bool ffOn = false;
bool ffStop = false;
bool isRecording = false;
bool playBackOn = false;

//Motor Speed
float timeout = 1.0;


//loadButton
std::string selectedWavFile;
int selectedNumber = -1;


//PORTAUDIO 
bool PAstatus = false;

// Parameters for WOW and FLUTTER effect
double wFdepth = 0.7;  // Example depth in milliseconds
double wFrate = 0.002;   // Example rate in Hz

Fl_Counter* timeCounter;

/****************************************************************************************************STRUCTS***********************************************************************************************/

struct dataBuffer
{
    // Struct members for HighEQ coefficients
    double a0 = 0, a1 = 0, a2 = 0;
    double b1 = 0, b2 = 0;
    double x1 = 0, x2 = 0;
    double y1 = 0, y2 = 0;

    // Struct members for midEQ coefficients
    double mid_a0 = 0, mid_a1 = 0, mid_a2 = 0;
    double mid_b1 = 0, mid_b2 = 0;
    double mid_x1 = 0, mid_x2 = 0;
    double mid_y1 = 0, mid_y2 = 0;

    // Struct members for lowEQ coefficients
    double low_a0 = 0, low_a1 = 0, low_a2 = 0;
    double low_b1 = 0, low_b2 = 0;
    double low_x1 = 0, low_x2 = 0;
    double low_y1 = 0, low_y2 = 0;

    std::vector<float> recordedSamples;
    int dataChannel = 0;

    // Member functions
    void calculateNotchCofficients(double gain_db, double Q, int fc, int fs, int flag);
    void calculateBoostCofficients(double gain_db, double Q, int fc, int fs, int flag);
};

//Holds the playback data which used by the PlayBack Stream
struct PlaybackData
{
    // Struct members
    const AudioFile<float>* audioFile;
    unsigned long currentFrame;
    unsigned long startTime;
    unsigned long endTime;
    unsigned long currentTime;
    unsigned int playBackChannel;
};

void dataBuffer::calculateNotchCofficients(double gain_db, double Q, int fc, int fs, int flag)
{
    if (flag == 1)
    {
        double K = 2.0 * pi * fc / fs;
        double V0 = pow(10.0, gain_db / 20.0);
        double d0 = 1.0 + K / Q + pow(K, 2.0);

        double a = 1.0 + K / (V0 * Q) + pow(K, 2.0);
        double b = 2.0 * (pow(K, 2.0) - 1.0);
        double g = 1.0 - K / Q + pow(K, 2.0);
        double d = 1.0 - K / (V0 * Q) + pow(K, 2.0);
        a0 = d0 / a;
        a1 = b / a;
        a2 = g / a;
        b1 = b / a;
        b2 = d / a;
    }
    else if (flag == 2)
    {
        double mid_K = 2.0 * pi * fc / fs;
        double mid_V0 = pow(10.0, gain_db / 20.0);
        double mid_d0 = 1.0 + mid_K / Q + pow(mid_K, 2.0);
   
        double mid_a = 1.0 + mid_K / (mid_V0 * Q) + pow(mid_K, 2.0);
        double mid_b = 2.0 * (pow(mid_K, 2.0) - 1.0);
        double mid_g = 1.0 - mid_K / Q + pow(mid_K, 2.0);
        double mid_d = 1.0 - mid_K / (mid_V0 * Q) + pow(mid_K, 2.0);
        mid_a0 = mid_d0 / mid_a;
        mid_a1 = mid_b / mid_a;
        mid_a2 = mid_g / mid_a;
        mid_b1 = mid_b / mid_a;
        mid_b2 = mid_d / mid_a;
    }
    else if (flag == 3)
    {
        // Calculate lowEQ coefficients
        double low_K = 2.0 * pi * fc / fs;
        double low_V0 = pow(10.0, gain_db / 20.0);
        double low_d0 = 1.0 + low_K / Q + pow(low_K, 2.0);
        double low_a = 1.0 + low_K / (low_V0 * Q) + pow(low_K, 2.0);
        double low_b = 2.0 * (pow(low_K, 2.0) - 1.0);
        double low_g = 1.0 - low_K / Q + pow(low_K, 2.0);
        double low_d = 1.0 - low_K / (low_V0 * Q) + pow(low_K, 2.0);
        low_a0 = low_d0 / low_a;
        low_a1 = low_b / low_a;
        low_a2 = low_g / low_a;
        low_b1 = low_b / low_a;
        low_b2 = low_d / low_a;
    }
}

void dataBuffer::calculateBoostCofficients(double gain_db, double Q, int fc, int fs, int flag)
{
    if (flag == 1)
    {
        double K = 2.0 * pi * fc / fs;
        double V0 = pow(10.0, gain_db / 20.0);
        double d0 = 1.0 + K / Q + pow(K, 2.0);
        double a = 1.0 + (V0 * K) / Q + pow(K, 2.0);
        double b = 2.0 * (pow(K, 2.0) - 1.0);
        double g = 1.0 - (V0 * K) / Q + pow(K, 2.0);
        double d = 1.0 - K / Q + pow(K, 2.0);
        a0 = a / d0;
        a1 = b / d0;
        a2 = g / d0;
        b1 = b / d0;
        b2 = d / d0;
    }

    else if (flag == 2)
    {
        double mid_K = 2.0 * pi * fc / fs;
        double mid_V0 = pow(10.0, gain_db / 20.0);
        double mid_d0 = 1.0 + mid_K / Q + pow(mid_K, 2.0);
        double mid_a = 1.0 + (mid_V0 * mid_K) / Q + pow(mid_K, 2.0);
        double mid_b = 2.0 * (pow(mid_K, 2.0) - 1.0);
        double mid_g = 1.0 - (mid_V0 * mid_K) / Q + pow(mid_K, 2.0);
        double mid_d = 1.0 - mid_K / Q + pow(mid_K, 2.0);
        mid_a0 = mid_a / mid_d0;
        mid_a1 = mid_b / mid_d0;
        mid_a2 = mid_g / mid_d0;
        mid_b1 = mid_b / mid_d0;
        mid_b2 = mid_d / mid_d0;
    }
    else if (flag == 3)
    {
        // Calculate lowEQ coefficients
        double low_K = 2.0 * pi * fc / fs;
        double low_V0 = pow(10.0, gain_db / 20.0);
        double low_d0 = 1.0 + low_K / Q + pow(low_K, 2.0);
        double low_a = 1.0 + (low_V0 * low_K) / Q + pow(low_K, 2.0);
        double low_b = 2.0 * (pow(low_K, 2.0) - 1.0);
        double low_g = 1.0 - (low_V0 * low_K) / Q + pow(low_K, 2.0);
        double low_d = 1.0 - low_K / Q + pow(low_K, 2.0);
        low_a0 = low_a / low_d0;
        low_a1 = low_b / low_d0;
        low_a2 = low_g / low_d0;
        low_b1 = low_b / low_d0;
        low_b2 = low_d / low_d0;
    }

   

}

//Stores the Reverb Dial values (which controls the amplitude of the reverb) 
//larger buffers allow for longer decays.
class Reverb
{
public:
    std::vector<double> buffer;
    unsigned int bufferSize;
    unsigned int bufferIndex;
    double decay;
    double volume[7] = { 0, 0, 0, 0, 0, 0, 0 };

    Reverb(unsigned int bufferSize, double decay) : bufferSize(bufferSize), decay(decay), bufferIndex(0), volume()
    {
        buffer.resize(bufferSize, 0.0);

    }

    double processSample(double input, int channel)
    {
        double output = buffer[bufferIndex];
        buffer[bufferIndex] = input + decay * output;
        bufferIndex = (bufferIndex + 1) % bufferSize;

        // Volume Reduction from Dials
        output *= volume[channel];

        return output;
    }
};




//Holds the Session info, this information is passed to the CreateDocument function which writes the txt file which saves its information. During the runtime lifetime, our SessionData is stored here.
struct ProjectInfo
{
    std::string projectName;
    std::string samplingRate;
    std::string inputChannels;
    std::string outputChannels;
    std::string channelNames[8] = { "Channel1", "Channel2", "Channel3", "Channel4", "Channel5", "Channel6", "Channel7", "Channel8" };
    std::string channelLocations[8];
    unsigned long channelLengths[8];
    unsigned long channelStartTimes[8];
    bool recPlayBackState[8] = { false, false, false, false, false, false, false, false };
    float sliderVolume[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    dataBuffer* dataPass[8];

};

//This stores Frequency and Gain values from the FLTK Dials. DialData written by the Dials and read by the recordCallback. 
struct dialData
{

    double saturationGain[8] = { 0,0,0,0,0,0,0,0 };

    //High
    double High_gain_db[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // Set the gain in dB
    double High_Q = 2.0; // Set the quality factor
    int High_fc[8] = { 15000, 15000, 15000, 15000, 15000, 15000, 15000, 15000 }; // Set the center frequency

    //Mid
    double Mid_gain_db[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // Set the gain in dB
    double Mid_Q = 2.0; // Set the quality factor
    int Mid_fc[8] = { 5000, 5000,5000,5000,5000,5000,5000,5000 }; // Set the center frequency

    //Low
    double Low_gain_db[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // Set the gain in dB
    double Low_Q = 2.0; // Set the quality factor
    int Low_fc[8] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 }; // Set the center frequency

};

//Global Class Declaration. 

//A PortAudio defined struct which holds the Audio Device information
const   PaDeviceInfo* deviceInfo;

ProjectInfo project;

dialData DialData;

Reverb reverb(10000, 1.0);


 /**********************************************************************************************COSMO I/O**************************************************************************************************/

/*Intializes Port Audio Library*/
void intialize(PaError& err);

/*Terminates Port Audio*/
void terminate(PaError& err);

/*Establishes Contact with the Audio Device, sets Input and Output Parameters that will be used by the audio Stream*/
void StreamSetup(PaError& err);

/*Opens the Stream and runs the audio processing loop*/
void openStream(dataBuffer& datapass, PaError& err);

//All Playback is handled here. 
static int playbackCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
    void* userData);

//Playback async thread -> sets up playback callback
void playbackThread(const std::string& filePath, unsigned long startTime, unsigned long endTime, int playbackChannel);

//Recording and the EQ/saturation/Reverb processing is handled in this function.
static int recordCallBack(const void* input, void* output, unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
    void* userData);

//processes samples via the In value, returns a double that is the processed signal. Alpha value provided by the saturation Dial i-> amount of tape sat. 
double tapeSaturationFunction(double in, double alpha);


/*************************************************************************Convolution.h********************************************************************************/

/*Convolves the Input Signal with the IR, mixes the convolved signal with White Noise and Writes it to Processed Folder*/
void convolution(int channelName, int load);

/*Writes the data contained in vector<float> recordedSamples; from the dataBuffer struct to a WAV file. The WAV file is stored in the RAW folder*/
void WriteAudio(dataBuffer& datapass, int channel2Write);

//Vector which handles the WOW and Flutter Processing
std::vector<double> WOW_and_FLUTTER_Function(const std::vector<double>& x, int SAMPLERATE, double Modfreq, double Width);

 /********************************************************************COSMO FILE MANAGER*****************************************************************************/
 
//Called in the Session Setup Function. This Creates the RAW and PROCESSED folders in a User specified directory.
void createFolders(const std::string& path);

//Whenever we update the Session Data, which is contained in the Project Struct, we call this function which creates a .txt file that holds the Session Data.
void createDocument(const std::string& path, const ProjectInfo& project);

//Modifies specific parameters within the .txt file.
void appendParameter(std::ofstream& file, const std::string& parameterName, const std::string& value);

//Called following the Create Folders function, this creates the intial and first Session Data
void createSessionData();

//Offloads Session Data from a .txt file and passes its value to the project struct.  
bool parseProjectInfo(const std::string& filePath, ProjectInfo& project);

/*************************************************************************COSMO UI*************************************************************************************/


//Logic for Opening or Creating a Project
void choiceButtonCallback(Fl_Widget* widget, void* data);

/*The Send and EQ Dials store their states and values in the Dial Data struct. We pass the channel corrosponding to the dial into the "int channel" parameters of their constructor and
callback functions. */

//This creates the Dial Group which manages the EQ Dial hierarchy and draws the EQ dials to the Screen
void EQDials(int x, int y, int w, int h, int LowDialColor, int HighDialColor, int channel);

//automates the creation of the send dials. 
void Send_Dials(int x, int y, int w, int h, Fl_PNG_Image* SendBG, int channel);

void setHighFrequencySweep(double freq, int channel);

void highEqCallback(Fl_Widget* widget, void*);

void setHighGain(double gain, int channel);

void highGainCallback(Fl_Widget* widget, void*);

void setMidFrequencySweep(double freq, int channel);

void midEqCallback(Fl_Widget* widget, void*);

void setMidGain(double gain, int channel);

void midGainCallback(Fl_Widget* widget, void*);

void setLowFrequencySweep(double freq, int channel);

void LowEqCallback(Fl_Widget* widget, void*);

void setLowGain(double gain, int channel);

void lowGainCallback(Fl_Widget* widget, void*);


//TO DO: Should we call WriteAudio and Terminate here rather then at STOP? 
//async thread calls the openStream function for Audio Recording. This is located in the record button callback. 
void openStreamAsync(dataBuffer& datapass, PaError& err);

//Intializes PlayBack via a PlayBackThread. 
void playButtonCallback(Fl_Widget* widget, void*);

//Stops Playback and Recording, calls WriteAudio Function, calls CreateDocument Function, updates Session Data via project Struct.  
void stopButtonCallback(Fl_Widget* widget, void*);

//increments the clock forward at double speed
void fastForwardButtonCallback(Fl_Widget* widget, void*);

//Loads a new Session via the parseSessionData Function or passes in a wav file to be Convolved then to a Playback Channel.
void loadButtonCallback(Fl_Widget* widget, void*);

//Gateway into the Application. Cues the User to select whether they are creating a new project or opening an existing project.
//Intializes PortAudio, sets up the Stream Parameters.
void sessionSetup();

//handles Data Callback for the Volume Slider
void sliderVolumeCallback(Fl_Widget* widget, void* channelNumber);

//creates the Volume Sliders, passes to the sliderVolumeCallback
void createVolumeSlider(int x, int y, int w, int h, int channelNumber);

//cleans up heap allocated memory
void exitCallback();

//Handles saturation amplitude for value "alpha" in the tape saturation function
void saturationDialCallback(Fl_Widget* widget, void* userData);

//Controls the reverb amplitude
void reverbDialCallback(Fl_Widget* widget, void* userData);

Fl_Double_Window* mainWindow();

/********************************************************COSMO_CLOCK.H*************************************************************/

void updateTime(void*);







