#pragma once
//STD
#include <iostream>
#include <conio.h>
#include <random>
#include <vector>
#include <cmath>
#include <thread>
#include "dirent.h" // need? 
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

#include <atomic>

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
#include <FL/fl_draw.H>



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

int ChannelNum = 0;
double sRate = 44100;
const double pi = 3.14159265358979323846;

//High
double High_gain_db = -15.0; // Set the gain in dB
double High_Q = 2.0; // Set the quality factor
int High_fc = 1000; // Set the center frequency

//Mid
double Mid_gain_db = 15.0; // Set the gain in dB
double Mid_Q = 2.0; // Set the quality factor
int Mid_fc = 180; // Set the center frequency

//Low
double Low_gain_db = 15.0; // Set the gain in dB
double Low_Q = 2.0; // Set the quality factor
int Low_fc = 180; // Set the center frequency

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

struct dataBuffer
{

    double a0 = 0, a1 = 0, a2 = 0;
    double b1 = 0, b2 = 0;
    double x1 = 0, x2 = 0;
    double y1 = 0, y2 = 0;

    
    void calculateNotchCofficients(double gain_db, double Q, int fc, int fs) // CUT
    {
        double K = 2.0 * pi * fc / fs;
        double V0 = pow(10.0, gain_db / 20.0);
        double d0 = 1.0 + K / Q + pow(K, 2.0);
        double a = 1.0 + K / (V0*Q) + pow(K, 2.0);
        double b = 2.0*(pow(K, 2.0) - 1.0);
        double g = 1.0 - K / Q + pow(K, 2.0);
        double d = 1.0 - K / (V0*Q) + pow(K, 2.0);
        a0 = d0/a;
        a1 = b / a;
        a2 = g / a;
        b1 = b / a;
        b2 = d / a;
    }
    

    void calculateBoostCofficients(double gain_db, double Q, int fc, int fs) // Boost
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

    std::vector<float> recordedSamples;
    int dataChannel = 0;

};

struct ProjectInfo
{
    std::string projectName;
    std::string samplingRate;
    std::string inputChannels;
    std::string outputChannels;
    std::string channelNames[8] = {"Channel1", "Channel2", "Channel3", "Channel4", "Channel5", "Channel6", "Channel7", "Channel8"};
    std::string channelLocations[8];
    unsigned long channelLengths[8];
    unsigned long channelStartTimes[8];
    bool recPlayBackState[8] = { false, false, false, false, false, false, false, false };
    float sliderVolume[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    dataBuffer* dataPass[8];

}; ProjectInfo project;


struct PlaybackData 
{
    const AudioFile<float>* audioFile;
    unsigned long currentFrame;
    unsigned long startTime;
    unsigned long endTime;
    unsigned long currentTime;
};

static int playbackCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    float* out = static_cast<float*>(outputBuffer);
    PlaybackData* playbackData = static_cast<PlaybackData*>(userData);

    const auto& audioBuffer = playbackData->audioFile->samples[0]; // Assuming mono audio


    if (playbackData->currentTime < playbackData->startTime)
    {

        //do nothing

    }
    else if (playbackData->currentTime >= playbackData->startTime && playbackData->currentFrame < playbackData->endTime)
    {
        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            // copy the audio data from the buffer to the output
            *out++ = audioBuffer[playbackData->currentFrame++] * project.sliderVolume[1];
        }
    }
    else if (playbackData->currentFrame >= playbackData->endTime)
    {

        // PLAYBACK COMPLETE
        return paComplete;
    }

    return paContinue;
}

void playbackThread(const std::string& filePath, unsigned long startTime, unsigned long endTime) //channel locations enabled for playback
{
  
    int framesPerBuffer = 256;

    // Load the WAV file
    std::cout << "PLAYBACK LOAD PATH: " << filePath << std::endl;
    AudioFile<float> audioFile;

    audioFile.load(filePath);
   // std::cout << "test to see before or after filepath" << std::endl;

    // Initialize PortAudio and open a stream
    PaStream* audioStream;

    PlaybackData playbackData;
    playbackData.audioFile = &audioFile;
    

    playbackData.currentTime = (hour * 3600 + minute * 60 + second) * sRate;
    playbackData.startTime = startTime;
    playbackData.endTime = endTime;

    playbackData.currentFrame = playbackData.currentTime - playbackData.startTime;
    playbackData.currentFrame = std::min(playbackData.currentFrame, playbackData.endTime);

    std::cout << playbackData.currentTime << std::endl;

    std::cout << playbackData.currentFrame << std::endl;
    
    std::cout << playbackData.startTime << std::endl;

    std::cout << playbackData.endTime << std::endl;


    // Open the stream
    err = Pa_OpenStream(
        &audioStream,
        &inputParameters,
        &outputParameters,
        sRate,
        framesPerBuffer,
        paNoFlag,
        playbackCallback, // Set the callback function
        &playbackData // Pass the playback data as user data
    );

    if (err != paNoError) {
        std::cout << "Error opening PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    // Start the stream
    err = Pa_StartStream(audioStream);
    if (err != paNoError) {
        std::cout << "Error starting PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
        return;
    }

    while (Pa_IsStreamActive(audioStream) && playBackOn)
    {
        Pa_Sleep(10); // Sleep for a short duration before checking again
    }

    // Stop and close the stream
    err = Pa_StopStream(audioStream);
    if (err != paNoError) {
        std::cout << "Error stopping PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
    }

    err = Pa_CloseStream(audioStream);
    if (err != paNoError) {
        std::cout << "Error closing PortAudio stream: " << Pa_GetErrorText(err) << std::endl;
    }
}



//This functions as a net, its catch must be passed to a struct. 
static int callBack1
(const void* input,
    void* output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{


   dataBuffer* datapass = (dataBuffer*)userData;
   float* inputSamples = (float*)input;
   float* outputSamples = (float*)output;



    if (ChannelNum > 1)
    {
        for (unsigned int i = 0; i < frameCount; i++)
        {
            for (int channel = 0; channel < 2; channel++)
            {
                float sample = inputSamples[i * 2 + channel];  // Assuming stereo audio
                datapass->recordedSamples.push_back(sample);
            }
        }
    }

    else if (ChannelNum == 1)
    {
        for (unsigned int i = 0; i < frameCount; i++)
        {

            if (High_gain_db >= 0.0)
            {
                
                double x0 = inputSamples[i];
                if (i == 0)
                {
                    /*I don't think we need to calculate the frequencies during each iteration,
                    we set them and they remain constant till the user changes them. But I should check if this
                    actually slows everything down and if impedes real time alteration of the EQ. But this should also be recallibrated 
                    whenver the filter is changed, which makes it unusuable during normal funcitoning... 
                    */
                   datapass->calculateBoostCofficients(High_gain_db, High_Q, High_fc, sRate);
                }
            

                double y0 = datapass->a0 * x0 + datapass->a1 * datapass->x1 + datapass->a2 * datapass->x2
                    - datapass->b1 * datapass->y1 - datapass->b2 * datapass->y2;

                datapass->recordedSamples.push_back(y0);

                outputSamples[i] = static_cast<float>(y0);

                // Update state variables
                datapass->x2 = datapass->x1;
                datapass->x1 = x0;
                datapass->y2 = datapass->y1;
                datapass->y1 = y0;
                

                // datapass->recordedSamples.push_back(inputSamples[i]);
                 
            }
            else if (High_gain_db < 0) // Calculate Notch Filter
            {

                double x0 = inputSamples[i];

                if (i == 0)
                {
                    datapass->calculateNotchCofficients(High_gain_db, High_Q, High_fc, sRate);
                }

                double y0 = datapass->a0 * x0 + datapass->a1 * datapass->x1 + datapass->a2 * datapass->x2
                    - datapass->b1 * datapass->y1 - datapass->b2 * datapass->y2;

               datapass->recordedSamples.push_back(y0);
               outputSamples[i] = static_cast<float>(y0);
               
               // Update state variables
                datapass->x2 = datapass->x1;
                datapass->x1 = x0;
                datapass->y2 = datapass->y1;
                datapass->y1 = y0;
                  
            }
        }
    }

    return paContinue;

} 


 /****************************************************************************COSMO I/O****************************************************************************/

/*Intializes Port Audio Library*/
void intialize(PaError& err);

/*Terminates Port Audio*/
void terminate(PaError& err);

/*Establishes Contact with the Audio Device, sets Input and Output Parameters that will be used by the audio Stream*/
void StreamSetup(PaError& err);

/*Opens the Stream and runs the audio processing loop*/
void openStream(dataBuffer& datapass, PaError& err);

/*************************************************************************Convolution.h********************************************************************************/

/*Convolves the Input Signal with the IR, mixes the convolved signal with White Noise and Writes it to Processed Folder*/
void convolution(int channelName);

/*Writes the data contained in vector<float> recordedSamples; from the dataBuffer struct to a WAV file. The WAV file is stored in the RAW folder*/
void WriteAudio(dataBuffer& datapass);

//LEGACY CODE: 
void readFile();

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

void setHighFrequencySweep(double freq);

void highEqCallback(Fl_Widget* widget, void*);

void setHighGain(double gain);

void highGainCallback(Fl_Widget* widget, void*);

void setMidFrequencySweep(double freq);

void midEqCallback(Fl_Widget* widget, void*);

void setMidGain(double gain);

void midGainCallback(Fl_Widget* widget, void*);

void setLowFrequencySweep(double freq);

void LowEqCallback(Fl_Widget* widget, void*);

void setLowGain(double gain);

void lowGainCallback(Fl_Widget* widget, void*);

void EQDials(int x, int y, int w, int h, int LowDialColor, int HighDialColor);
 
void Send_Dials(int x, int y, int w, int h, Fl_PNG_Image* SendBG);

//TO DO: Should we call WriteAudio and Terminate here rather then STOP? 
//async thread calls the openStream function for Audio Recording. This is located in the record button callback. 
void openStreamAsync(dataBuffer& datapass, PaError& err);

//Intializes PlayBack via a PlayBackThread. 
void playButtonCallback(Fl_Widget* widget, void*);

//Stops Playback and Recording, calls WriteAudio Function, calls CreateDocument Function, updates Session Data via project Struct.  
void stopButtonCallback(Fl_Widget* widget, void*);

void fastForwardButtonCallback(Fl_Widget* widget, void*);

//Gateway into the Application. Cues the User to select whether they are creating a new project or opening an existing project.
//Intializes PortAudio, sets up the Stream Parameters.
void sessionSetup();

//handles Data Callback for the Volume Slider
void sliderVolumeCallback(Fl_Widget* widget, void* channelNumber);

//creates the Volume Sliders, passes to the sliderVolumeCallback
void createVolumeSlider(int x, int y, int w, int h, int channelNumber);

//cleans up heap allocated memory
void exitCallback();

Fl_Window* mainWindow();

/********************************************************COSMO_CLOCK.H*************************************************************/

void updateTime(void*);



/* 

std::vector<double> WOW_and_FLUTTER_Function(const std::vector<double>& x, int SAMPLERATE, double Modfreq, double Width) {
    int WIDTH = static_cast<int>(std::round(Width * SAMPLERATE));
    double MODFREQ = Modfreq / SAMPLERATE;
    int LEN = x.size();
    int L = 2 + WIDTH + WIDTH * 2;
    std::vector<double> Delayline(L, 0.0);
    std::vector<double> y(LEN, 0.0);

    for (int n = 0; n < LEN; n++) {
        double MOD = std::sin(MODFREQ * 2 * pi * n);
        double TAP = 1 + WIDTH + WIDTH * MOD;
        int i = static_cast<int>(std::floor(TAP));
        double frac = TAP - i;
        Delayline.insert(Delayline.begin(), x[n]);
        Delayline.resize(L);

        // Linear Interpolation
        y[n] = Delayline[i + 1] * frac + Delayline[i] * (1 - frac);
    }

    return y;
}

const std::string Input_filename = "C:\\Users\\alcin\\Desktop\\flutter\\guitar.wav";
    const std::string Output_filename = "C:\\Users\\alcin\\Desktop\\flutter\\flutterr.wav";

    AudioFile<float> audio;

    if (!audio.load(Input_filename)) {
        std::cerr << "Failed to open input audio file." << std::endl;
        std::cout << Input_filename << std::endl;
        exit(1);
    }
    else {
        std::cout << "Input Loaded" << std::endl;
    }

    // Extract audio data
    std::vector<float> audioData = audio.samples[0];

    // Parameters for WOW and FLUTTER effect
    int sampleRate = audio.getSampleRate();
    double depth = 0.5;  // Example depth in milliseconds
    double rate = 0.002;   // Example rate in Hz

    // Convert audioData to std::vector<double>
    std::vector<double> audioDataDouble(audioData.begin(), audioData.end());

    // Apply WOW and FLUTTER effect
    std::vector<double> processedData = WOW_and_FLUTTER_Function(audioDataDouble, sampleRate, depth, rate);

    // Create a multi-channel buffer with a single channel
    AudioFile<float>::AudioBuffer buffer;
    buffer.resize(1);
    buffer[0] = std::vector<float>(processedData.begin(), processedData.end());

    // Create an AudioFile object
    AudioFile<float> audioFile;

    // Set the audio data
    audioFile.setAudioBuffer(buffer);

    // Set the sample rate and number of channels
    audioFile.setSampleRate(sampleRate);
    audioFile.setNumChannels(1);  // Mono audio

    // Write the audio data to the WAV file
    if (!audioFile.save(Output_filename)) {
        std::cerr << "Failed to write audio file." << std::endl;
        exit(1);
    }

    std::cout << "Audio file saved: " << Output_filename << std::endl;
}

void stopButtonCallback(Fl_Widget* widget, void* data)
{
    bool exitLoop = false;

    rewindOn = false;
    rewindStop = false;
    ffOn = false;
    ffStop = false;
    

    TransportButtons* transportButtons = static_cast<TransportButtons*>(data);

    transportButtons->play->color(FL_WHITE);
    transportButtons->rewind->color(FL_WHITE);
    transportButtons->fastForward->color(FL_WHITE);
    transportButtons->record->color(FL_WHITE);

    Fl::redraw();



    if (exitLoop == false && isRecording == true)
    {
        //PROJECTPATH IS NORMALLY TO PARENT FOLDER
        filename = "test.txt";
      //  filePath = "C:\\Users\\alcin\\Desktop\\" + filename;
        isRecording = false; // stops the Recorder
        std::cout << "Closing Stream and Ending Recording" << std::endl;
   
        WriteAudio(datapass);
        convolution();
        createDocument(projectPath, project);
        exitLoop = true;

    }
    else if (isRecording == false)
    {
        std::cout << "Stoping Transport" << std::endl;




        std::vector<double> WOW_and_FLUTTER_Function(const std::vector<double>& x, int SAMPLERATE, double Modfreq, double Width) {
    int WIDTH = static_cast<int>(std::round(Width * SAMPLERATE));
    double MODFREQ = Modfreq / SAMPLERATE;
    int LEN = x.size();
    int L = 2 + WIDTH + WIDTH * 2;
    std::vector<double> Delayline(L, 0.0);
    std::vector<double> y(LEN, 0.0);

    for (int n = 0; n < LEN; n++) {
        double MOD = std::sin(MODFREQ * 2 * pi * n);
        double TAP = 1 + WIDTH + WIDTH * MOD;
        int i = static_cast<int>(std::floor(TAP));
        double frac = TAP - i;
        Delayline.insert(Delayline.begin(), x[n]);
        Delayline.resize(L);

        // Linear Interpolation
        y[n] = Delayline[i + 1] * frac + Delayline[i] * (1 - frac);
    }

    return y;
}

    }*/


/*static int playbackCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    float* out = static_cast<float*>(outputBuffer);
    PlaybackData* playbackData = static_cast<PlaybackData*>(userData);

    const auto& audioBuffer = playbackData->audioFile->samples[0]; // Assuming mono audio

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        // Check if we have reached the end of the audio buffer
        if (playbackData->currentFrame >= audioBuffer.size())
        {
            // If we have reached the end, fill the output buffer with silence
            *out++ = 0.0f;
        }
        else
        {
            // Otherwise, copy the audio data from the buffer to the output
            *out++ = audioBuffer[playbackData->currentFrame++] * project.sliderVolume[1];
        }
    }

    return paContinue;
}*/