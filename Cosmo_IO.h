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
            *out++ = audioBuffer[playbackData->currentFrame++] * project.sliderVolume[playbackData->playBackChannel];
        }
    }
    else if (playbackData->currentFrame >= playbackData->endTime)
    {
     
       return paComplete;

      
    }

    return paContinue;
}



void playbackThread(const std::string& filePath, unsigned long startTime, unsigned long endTime, int playbackChannel) //channel locations enabled for playback
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
    playbackData.playBackChannel = playbackChannel;
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
static int recordCallBack
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
            double x0 = inputSamples[i];
            double y0 = 0;
            double z0 = 0;
            double Lz0 = 0;

            
            if (DialData.High_gain_db[datapass->dataChannel] >= 0)
            {


                if (i == 0)
                {

                    datapass->calculateBoostCofficients(DialData.High_gain_db[datapass->dataChannel], DialData.High_Q, DialData.High_fc[datapass->dataChannel], sRate, 1);
                }

                //end
                y0 = datapass->a0 * x0 + datapass->a1 * datapass->x1 + datapass->a2 * datapass->x2
                    - datapass->b1 * datapass->y1 - datapass->b2 * datapass->y2;

                // Update state variables
                datapass->x2 = datapass->x1;
                datapass->x1 = x0;
                datapass->y2 = datapass->y1;
                datapass->y1 = y0;

            }
            else if(DialData.High_gain_db[datapass->dataChannel] < 0)
            {

                if (i == 0)
                {

                    datapass->calculateNotchCofficients(DialData.High_gain_db[datapass->dataChannel], DialData.High_Q, DialData.High_fc[datapass->dataChannel], sRate, 1);
                }

                //end
                y0 = datapass->a0 * x0 + datapass->a1 * datapass->x1 + datapass->a2 * datapass->x2
                    - datapass->b1 * datapass->y1 - datapass->b2 * datapass->y2;

                // Update state variables
                datapass->x2 = datapass->x1;
                datapass->x1 = x0;
                datapass->y2 = datapass->y1;
                datapass->y1 = y0;


            }
            
            if (DialData.Mid_gain_db[datapass->dataChannel] >= 0)
            {
                if (i == 0)
                {
                    datapass->calculateBoostCofficients(DialData.Mid_gain_db[datapass->dataChannel], DialData.Mid_Q, DialData.Mid_fc[datapass->dataChannel], sRate, 2);
                }

                // End
                z0 = datapass->mid_a0 * x0 + datapass->mid_a1 * datapass->mid_x1 + datapass->mid_a2 * datapass->mid_x2
                    - datapass->mid_b1 * datapass->mid_y1 - datapass->mid_b2 * datapass->mid_y2;

                // Update state variables
                datapass->mid_x2 = datapass->mid_x1;
                datapass->mid_x1 = x0;
                datapass->mid_y2 = datapass->mid_y1;
                datapass->mid_y1 = z0;
            }
            else if (DialData.Mid_gain_db[datapass->dataChannel] < 0)
            {
                if (i == 0)
                {
                    datapass->calculateNotchCofficients(DialData.Mid_gain_db[datapass->dataChannel], DialData.Mid_Q, DialData.Mid_fc[datapass->dataChannel], sRate, 2);
                }

                // End
                z0 = datapass->mid_a0 * x0 + datapass->mid_a1 * datapass->mid_x1 + datapass->mid_a2 * datapass->mid_x2
                    - datapass->mid_b1 * datapass->mid_y1 - datapass->mid_b2 * datapass->mid_y2;

                // Update state variables
                datapass->mid_x2 = datapass->mid_x1;
                datapass->mid_x1 = x0;
                datapass->mid_y2 = datapass->mid_y1;
                datapass->mid_y1 = z0;
            }

            if (DialData.Low_gain_db[datapass->dataChannel] >= 0)
            {
                if (i == 0)
                {
                    datapass->calculateBoostCofficients(DialData.Low_gain_db[datapass->dataChannel], DialData.Low_Q, DialData.Low_fc[datapass->dataChannel], sRate, 3);
                }

                // End
                Lz0 = datapass->low_a0 * x0 + datapass->low_a1 * datapass->low_x1 + datapass->low_a2 * datapass->low_x2
                    - datapass->low_b1 * datapass->low_y1 - datapass->low_b2 * datapass->low_y2;

                // Update state variables
                datapass->low_x2 = datapass->low_x1;
                datapass->low_x1 = x0;
                datapass->low_y2 = datapass->low_y1;
                datapass->low_y1 = Lz0;
            }
            else if (DialData.Low_gain_db[datapass->dataChannel] < 0)
            {
                if (i == 0)
                {
                    datapass->calculateNotchCofficients(DialData.Low_gain_db[datapass->dataChannel], DialData.Low_Q, DialData.Low_fc[datapass->dataChannel], sRate, 3);
                }

                // End
                Lz0 = datapass->low_a0 * x0 + datapass->low_a1 * datapass->low_x1 + datapass->low_a2 * datapass->low_x2
                    - datapass->low_b1 * datapass->low_y1 - datapass->low_b2 * datapass->low_y2;

                // Update state variables
                datapass->low_x2 = datapass->low_x1;
                datapass->low_x1 = x0;
                datapass->low_y2 = datapass->low_y1;
                datapass->low_y1 = Lz0;
            }


            double final = y0 * 0.33 + z0 * 0.33 + Lz0 * 0.33;

            datapass->recordedSamples.push_back(y0 * 0.33 + z0 * 0.33 + Lz0 * 0.33);

            outputSamples[i] = static_cast<float>(y0 + tapeSaturationFunction(y0, DialData.saturationGain[datapass->dataChannel]) + reverb.processSample(y0, datapass->dataChannel));


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
    dialog->hide();

}


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
    Fl_Window deviceList(650, 120, "Input Device Setup");
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
    Fl_Window outputList(650, 120, "Output Device Setup");
    Fl_Choice outputDevice(200, 30, 400, 30, "Choose a Output Device");

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
        &recordCallBack,
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

