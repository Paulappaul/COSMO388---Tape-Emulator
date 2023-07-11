#pragma once
#include "Cosmo.h"

double tapeSaturationFunction(double in, double alpha)
{
    return (2 / pi) * std::atan(in * alpha);
}

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
     
    }
    else if (playbackData->currentTime >= playbackData->startTime && playbackData->currentFrame < playbackData->endTime)
    {
        for (unsigned long i = 0; i < framesPerBuffer; ++i)
        {
            // Check if the current frame index exceeds the audioBuffer size
            if (playbackData->currentFrame >= audioBuffer.size())
                break;

            // Copy the audio data from the buffer to the output
            *out++ = audioBuffer[playbackData->currentFrame++] * project.sliderVolume[1];
        }
    }
    else if (playbackData->currentFrame >= playbackData->endTime)
    {
     
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

               
               // datapass->recordedSamples.push_back(y0);

               // outputSamples[i] = static_cast<float>(y0);


          
                double final = tapeSaturationFunction(y0, DialData.saturationGain[datapass->dataChannel]) + y0;

                datapass->recordedSamples.push_back(final);

                outputSamples[i] = static_cast<float>(final);
                

                // Update state variables
                datapass->x2 = datapass->x1;
                datapass->x1 = x0;
                datapass->y2 = datapass->y1;
                datapass->y1 = y0;


    

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

              //  datapass->recordedSamples.push_back(y0);
              //  outputSamples[i] = static_cast<float>(y0);

                double final = tapeSaturationFunction(y0, DialData.saturationGain[datapass->dataChannel]) + y0;

                datapass->recordedSamples.push_back(final);

                outputSamples[i] = static_cast<float>(final);

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

    std::cout << "datapass datachannel = " << datapass.dataChannel << std::endl;
    std::cout << DialData.saturationGain[datapass.dataChannel] << "this is the value!" << std::endl;

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

