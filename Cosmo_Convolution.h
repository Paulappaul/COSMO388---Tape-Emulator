#pragma once
#include "Cosmo.h"


void WriteAudio(dataBuffer& datapass, int channel2Write)
{

    std::cout << "Samples Passed: " << datapass.recordedSamples.size() << std::endl;

    // Create an AudioFile object
    AudioFile<float> audioFile;
    
    // Create a multi-channel buffer
    AudioFile<float>::AudioBuffer buffer;

    if (ChannelNum > 1)
    {
        buffer.resize(2);  // Resize the buffer to have 2 channels

        // Assign the recorded samples to the stereo channels

        for (size_t i = 0; i < datapass.recordedSamples.size(); i += 2)
        {
            buffer[0].push_back(datapass.recordedSamples[i]);       // Left channel
            buffer[1].push_back(datapass.recordedSamples[i + 1]);   // Right channel
        }
    }
    else if (ChannelNum == 1)
    {
        buffer.resize(1);
        for (size_t i = 0; i < datapass.recordedSamples.size(); i++)
        {
            buffer[0].push_back(datapass.recordedSamples[i]);       // Left channel
        }

    }

    audioFile.setAudioBuffer(buffer);

    // Set the sample rate and number of channels
    audioFile.setSampleRate(sRate);
    audioFile.setNumChannels(ChannelNum);  // Stereo audio

    std::cout << "writting channel: " << channel2Write << std::endl;

    std::string holder = project.channelNames[channel2Write];

    std::cout << holder << std::endl;
    const std::string writePath = projectPath + "RAW/" + project.channelNames[channel2Write] + ".wav";

    std::cout << writePath << std::endl;

    // Write the audio data to the WAV file
    if (!audioFile.save(writePath)) 
    {
        std::cerr << "Failed to write audio file." << std::endl;
        std::cout << "attempted to write: " << writePath << std::endl;
        exit(1);
    }

    std::cout << "RAW AUDIO file saved TOO: " << writePath << std::endl;

    //do I need to delete DataPass ?

}

void convolution(int channelName)
{
    FFTConvolver fft;

    const std::string Input_filename = projectPath + "RAW/" + project.channelNames[channelName] + ".wav";
    const std::string Impulse_filename = "C:\\Users\\alcin\\Desktop\\Convolution\\stream\\boy.wav";
    const std::string outputfilename = projectPath + "PROCESSED/" + project.channelNames[channelName] + ".wav";

    std::cout << "INPUT FILENAME CONVOLUTION: " << Input_filename << std::endl;

    // Open the audio file
    AudioFile<float> inputFile;
    AudioFile<float> impulsefile;

    if (!inputFile.load(Input_filename))
    {
        std::cerr << "Failed to open input audio file." << std::endl;
        std::cout << Input_filename << std::endl;
        exit(1);
    }
    else {
        std::cout << "Input Loaded" << std::endl;
    }

    if (!impulsefile.load(Impulse_filename)) {
        std::cerr << "Failed to open impulse audio file." << std::endl;
        exit(1);
    }
    else {
        std::cout << "Output Loaded" << std::endl;
    }

    // Read the audio data as floating-point values

    const std::vector<float>& inputData = inputFile.samples[0]; // Assuming mono audio
    const std::vector<float>& impulseData = impulsefile.samples[0]; // Assuming mono audio

    size_t inputlengthinSeconds = inputFile.getLengthInSeconds();
    int inputtotalsampleSize = inputFile.getNumSamplesPerChannel();

    size_t impulselengthinSeconds = impulsefile.getLengthInSeconds();
    int impulsetotalsampleSize = impulsefile.getNumSamplesPerChannel();

    // Prepare input 
    std::vector<fftconvolver::Sample>& in = inputFile.samples[0];

    // Prepare Impulse
    std::vector<fftconvolver::Sample>& ir = impulsefile.samples[0];

    std::vector<fftconvolver::Sample> out(inputtotalsampleSize);

    if (fft.init(1024, &ir[0], impulsetotalsampleSize) == true)
    {
        std::cout << "Processing" << std::endl;
        fft.process(&in[0], &out[0], inputtotalsampleSize);
    }
    else
    {
        std::cout << "Failed to initialize" << std::endl;
    }

    // Generate white noise at -56dB
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    std::vector<float> whiteNoise(inputtotalsampleSize);
    const float noiseLevel = -60.0f;  // Desired noise level in dB -56 natively
    const float noiseAmplitude = std::pow(10.0f, noiseLevel / 20.0f);

    for (size_t i = 0; i < inputtotalsampleSize; i++) {
        whiteNoise[i] = dist(gen) * noiseAmplitude;
    }

    // Convolve the white noise with the impulse response
    std::vector<float> noiseConvolved(inputtotalsampleSize);
    fft.process(&whiteNoise[0], &noiseConvolved[0], inputtotalsampleSize);

    // Add the output and the noise-convolved output
    std::vector<float> combinedSignal(inputtotalsampleSize);

    for (size_t i = 0; i < inputtotalsampleSize; i++) {
        combinedSignal[i] = out[i] + noiseConvolved[i];
    }

    // Normalize the combined signal to -12dB
    const float targetLevel = -12.0f;  // Target level in dB
    const float normalizationFactor = std::pow(10.0f, targetLevel / 20.0f);
    for (size_t i = 0; i < inputtotalsampleSize; i++) {
        combinedSignal[i] *= normalizationFactor;
    }

    // Create an AudioFile object
    AudioFile<float> audioFile;

    // Create a multi-channel buffer with a single channel
    AudioFile<float>::AudioBuffer buffer;
    buffer.resize(1);
    buffer[0] = combinedSignal;

    // Set the audio data
    audioFile.setAudioBuffer(buffer);

    // Set the sample rate and number of channels
    audioFile.setSampleRate(sRate);
    audioFile.setNumChannels(1);  // Mono audio

    // Write the audio data to the WAV file
    if (!audioFile.save(outputfilename)) {
        std::cerr << "Failed to write audio file." << std::endl;
        exit(1);
    }

    std::cout << "CONVOLUTE OUTPUT file saved: " << outputfilename << std::endl;

    project.channelLocations[channelName] = outputfilename;



}







void readFile()
{
    const std::string Filename_PRE = "C:\\Users\\alcin\\Desktop\\MonoW.wav";
    const std::string Filename_Write = "C:\\Users\\alcin\\Desktop\\Notch1000.wav";

    AudioFile<float> EQtoWrite;
    EQtoWrite.load(Filename_PRE);
    dataBuffer data;
    dataBuffer* datapass = &data;

    const std::vector<float>& inputData = EQtoWrite.samples[0];

    std::cout << "Samples: " << EQtoWrite.getNumSamplesPerChannel() << std::endl;
    std::cout << "VectorSize" << inputData.size() << std::endl;

    for (int i = 0; i < EQtoWrite.getNumSamplesPerChannel(); i++)
    {

        double x0 = inputData[i];
        if (High_gain_db >= 0 && i == 0)
        {
            datapass->calculateBoostCofficients(High_gain_db, High_Q, High_fc, sRate);
            std::cout << "Boost selected" << std::endl;
        }
        else if (High_gain_db < 0 && i == 0)
        {
            datapass->calculateNotchCofficients(High_gain_db, High_Q, High_fc, sRate);
            std::cout << "Notch selected" << std::endl;
        }
        double y0 = datapass->a0 * x0 + datapass->a1 * datapass->x1 + datapass->a2 * datapass->x2
            - datapass->b1 * datapass->y1 - datapass->b2 * datapass->y2;

        datapass->recordedSamples.push_back(y0);

        // Update state variables
        datapass->x2 = datapass->x1;
        datapass->x1 = x0;
        datapass->y2 = datapass->y1;
        datapass->y1 = y0;


    }

    // Create an AudioFile object
    AudioFile<float> audioFile;

    // Create a multi-channel buffer with a single channel
    AudioFile<float>::AudioBuffer buffer;
    buffer.resize(1);
    buffer[0] = datapass->recordedSamples;

    // Set the audio data
    audioFile.setAudioBuffer(buffer);

    // Set the sample rate and number of channels
    audioFile.setSampleRate(sRate);
    audioFile.setNumChannels(1);  // Mono audio

    // Write the audio data to the WAV file
    if (!audioFile.save(Filename_Write)) {
        std::cerr << "Failed to write audio file." << std::endl;
        exit(1);
    }

    std::cout << "Audio file saved: " << Filename_Write << std::endl;



}