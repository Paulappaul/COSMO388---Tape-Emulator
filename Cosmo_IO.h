#pragma once
#include "Cosmo.h"

void intialize(PaError& err)
{
    //Initalization 
    if (PAstatus == false)
    {
        err = Pa_Initialize();
        if (err != paNoError)
        {
            std::cout << Pa_GetErrorText(err) << std::endl;
            exit(1);

        }
        else
        {
            std::cout << "intialized successfully." << std::endl;
            PAstatus = true;
        }
    }
    else
    {
        std::cout << "PA already intialized" << std::endl;
    
    }

}

void terminate(PaError& err)
{

    //Termination 
    if (PAstatus == true)
    {
        err = Pa_Terminate();
        if (err != paNoError)
        {

            std::cout << Pa_GetErrorText(err) << std::endl;
            exit(1);

        }
        else { std::cout << "Terminated successfully" << std::endl; PAstatus = false; }
    }
    else { std::cout << "PA already termianted" << std::endl; }
}

//A struct which holds Device information
const   PaDeviceInfo* deviceInfo;

int inputDebug = 0; // remove when certain

void deviceChoiceButtonCallback(Fl_Widget* widget, void* data)
{
    Fl_Choice* inputChoice = static_cast<Fl_Choice*>(data);
    int selectedInputDevice = inputChoice->value();
    deviceInfo = Pa_GetDeviceInfo(selectedInputDevice);

    inputParameters.device = selectedInputDevice;
    inputParameters.channelCount = deviceInfo->maxInputChannels;
    inputParameters.hostApiSpecificStreamInfo = NULL; 
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;
    inputParameters.channelCount = 1;
    ChannelNum = 1;
    Fl_Window* dialog = static_cast<Fl_Window*>(inputChoice->parent());
    inputDebug = selectedInputDevice;
    dialog->hide();

}

int outputDebug = 0; //remove the certain

void outputChoiceButtonCallback(Fl_Widget* widget, void* data)
{
    Fl_Choice* outputChoice = static_cast<Fl_Choice*>(data);
    int selectedOutputDevice = outputChoice->value();
    deviceInfo = Pa_GetDeviceInfo(selectedOutputDevice);
    std::cout << selectedOutputDevice;
    //handle Outputs
    outputParameters.device = selectedOutputDevice;
    outputParameters.channelCount = 1; //deviceInfo->maxOutputChannels;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
    sRate = 44100;
    Fl_Window* dialog = static_cast<Fl_Window*>(outputChoice->parent());
    outputDebug = selectedOutputDevice;
    dialog->hide();

}

void StreamSetup(PaError& err)
{

    int numDevices;
    numDevices = Pa_GetDeviceCount();

    //Error Checking
    if (numDevices < 0)
    {
        printf("ERROR: Pa_CountDevices returned 0x%x\n", numDevices);
        err = numDevices;
        terminate(err);
    }


    //Input
    Fl_Window deviceList(650, 120, "Device Setup");
    Fl_Choice inputDevice(200, 30, 400, 30, "Choose a Input Device");


    //iterate throught the different devices
    for (int i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);
        inputDevice.add(deviceInfo->name);

    }

    Fl_Button deviceChoice(110, 80, 80, 30, "Choose");
    deviceChoice.callback(deviceChoiceButtonCallback, &inputDevice);
    deviceList.end();

    deviceList.show();
    while (deviceList.shown())
    {
        Fl::wait();
    }

    //Output
    Fl_Window outputList(400, 120, "Output Device Setup");
    Fl_Choice outputDevice(200, 30, 400, 25, "Choose a Output Device");

    for (int i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);

        std::string deviceName = deviceInfo->name;
        deviceName.erase(std::remove(deviceName.begin(), deviceName.end(), '/'), deviceName.end()); // "/" was causing errors in menu selection, so we remove them. 

        outputDevice.add(deviceName.c_str());
        std::cout << deviceName << std::endl;
    }

    Fl_Button outputChoice(110, 80, 80, 30, "Choose");
    outputChoice.callback(outputChoiceButtonCallback, &outputDevice);
    outputList.end();

    outputList.show();
    while (outputList.shown())
    {
        Fl::wait();
    }

    deviceInfo = Pa_GetDeviceInfo(inputDebug);
    std::cout << "NAME: " << deviceInfo->name << " SAMPLE RATE: "
        << deviceInfo->defaultSampleRate << " Max Input Channels: "
        << deviceInfo->maxInputChannels << " Max Output Channels: "
        << deviceInfo->maxOutputChannels << "input latency: "
        << deviceInfo->defaultLowInputLatency << "output latency: "
        << deviceInfo->defaultLowOutputLatency <<
        std::endl;

    //handle Outputs


    deviceInfo = Pa_GetDeviceInfo(outputDebug);
    std::cout << "NAME: " << deviceInfo->name << " SAMPLE RATE: "
        << deviceInfo->defaultSampleRate << " Max Input Channels: "
        << deviceInfo->maxInputChannels << " Max Output Channels: "
        << deviceInfo->maxOutputChannels << "input latency: "
        << deviceInfo->defaultLowInputLatency << "output latency: "
        << deviceInfo->defaultLowOutputLatency <<
        std::endl;


    

}

void openStream(dataBuffer& datapass, PaError& err)
{

    int framesPerBuffer = 256;
    PaStream* audioStream;


    err = Pa_OpenStream(
        &audioStream,
        &inputParameters,
        &outputParameters,
        sRate,
        framesPerBuffer,
        paNoFlag,
        &callBack1,
        &datapass
    );

    if (err != paNoError) { std::cout << "Error Opening Stream" << std::endl; }
    else { std::cout << "Stream opened" << std::endl; }

    //start stream and record
    err = Pa_StartStream(audioStream);
    if (err != paNoError) { std::cout << "Error Starting Stream" << std::endl; }
    else { std::cout << "stream started" << std::endl; }

while ((!_kbhit() || _getch() != ' ') || isRecording) // first condition is the space bar, second is a bool controlled by the stop button. 
{
        // Recording is in progress
    }
    if (err < 0) { std::cout << "Recording failed" << std::endl; }

    //close stream
    err = Pa_CloseStream(audioStream);
    if (err != paNoError) { std::cout << "Close Failed" << std::endl; }
    else
    {
        std::cout << "stream closed" << std::endl;
    }


}

//where recording is handled. 

void openStreamAsync(dataBuffer &datapass, PaError& err)
{
    std::thread audioThread([&]()
        {

            openStream(datapass, err);

 //   WriteAudio(datapass);

    // Terminate
   // terminate(err);

        });

    audioThread.detach();  // Detach the thread to let it run independently
}


/*

THIS IS THE TERMINAL LEGACY CODE FOR STREAMSETUP 


void StreamSetup(PaError& err)
{

    int numDevices;
    numDevices = Pa_GetDeviceCount();

    //Error Checking
    if (numDevices < 0)
    {
        printf("ERROR: Pa_CountDevices returned 0x%x\n", numDevices);
        err = numDevices;
        terminate(err);
    }
    std::cout << "DEVICES FOUND:: " << numDevices << std::endl;

    //A struct which holds Device information
    const   PaDeviceInfo* deviceInfo;

    //iterate throught the different devices
    for (int i = 0; i < numDevices; i++)
    {
        deviceInfo = Pa_GetDeviceInfo(i);
        std::cout << i << " " << deviceInfo->name << std::endl;

    }

    //USER REVIEW AND DEBUG
    int userChoice = 0;
    std::cout << "Choose a device to Review, 100 to exit" << std::endl;
    std::cin >> userChoice;
    while (userChoice != 100)
    {
        deviceInfo = Pa_GetDeviceInfo(userChoice);
        std::cout << "NAME: " << deviceInfo->name << " SAMPLE RATE: "
            << deviceInfo->defaultSampleRate << " Max Input Channels: "
            << deviceInfo->maxInputChannels << " Max Output Channels: "
            << deviceInfo->maxOutputChannels << "input latency: "
            << deviceInfo->defaultLowInputLatency << "output latency: "
            << deviceInfo->defaultLowOutputLatency <<
            std::endl;

        std::cout << "Choose a device to Review, 100 to exit" << std::endl;
        std::cin >> userChoice;
    }

    //Intialize a Device for Streaming
    std::cout << "Choose a Device to use for Streaming" << std::endl;
    std::cin >> userChoice;
    deviceInfo = Pa_GetDeviceInfo(userChoice);

    std::cout << "input device is: " << deviceInfo->name << std::endl;




    //handle Inputs
    inputParameters.device = userChoice;
    inputParameters.channelCount = deviceInfo->maxInputChannels;
    inputParameters.hostApiSpecificStreamInfo = NULL; //or &devinceInfo?
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = deviceInfo->defaultLowInputLatency;


    if (deviceInfo->maxInputChannels > 1)
    {

        std::cout << "Channel Count is larger then 1. Would you like to establish a Mono Stream? 1/0" << std::endl;
        std::cin >> userChoice;
        if (userChoice == 1)
        {
            inputParameters.channelCount = 1;
            ChannelNum = 1;
        }
        else { std::cout << "STREAM Channel count: " << deviceInfo->maxInputChannels << std::endl; ChannelNum = 2; }

    }


    std::cout << "Output Choice" << std::endl;

    std::cin >> userChoice;
    deviceInfo = Pa_GetDeviceInfo(userChoice);

    std::cout << "Output device is: " << deviceInfo->name << std::endl;

    //handle Outputs
    outputParameters.device = userChoice;
    outputParameters.channelCount = 1; //deviceInfo->maxOutputChannels;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;

    //Intialize Stream 
    sRate = deviceInfo->defaultSampleRate;
    std::cout << "Intializing Stream... Sample Rate is: " << sRate << std::endl;
    if (sRate < 48000)
    {
        std::cout << "Would you like to Switch to 48khz? 1/0" << std::endl;
        std::cin >> userChoice;
        if (userChoice == 1)
        {
            sRate = 48000;
        }
        else { std::cout << "Sample Rate: " << deviceInfo->defaultSampleRate << std::endl; }
    }

}*/

