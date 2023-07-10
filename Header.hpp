#pragma once
#include "Cosmo.h"

struct ProjectInfo
{
    std::string projectName;
    std::string samplingRate;
    std::string inputChannels;
    std::string outputChannels;
    std::string channelNames[8];
    std::string channelLocations[8];
    std::string channelLengths[8];
    std::string channelStartTimes[8];
};

// Function to append or replace a parameter value in the document
void appendParameter(std::ofstream& file, const std::string& parameterName, const std::string& value)
{
    file << parameterName << ": " << value << "\n";
}

// Function to create the document
void createDocument(const std::string& path, const ProjectInfo& project)
{
    std::ofstream file(path);

    if (file.is_open())
    {
        appendParameter(file, "Project Name", project.projectName);
        appendParameter(file, "Sampling Rate", project.samplingRate);
        appendParameter(file, "Input Channels", project.inputChannels);
        appendParameter(file, "Output Channels", project.outputChannels);

        for (int i = 0; i < 8; ++i)
        {
            std::string channelInfo = "Channel_" + std::to_string(i + 1) + "[" +
                project.channelNames[i] + "_" +
                project.channelLocations[i] + "_" +
                project.channelLengths[i] + "_" +
                project.channelStartTimes[i] + "]";
            appendParameter(file, channelInfo, "");
        }

        file.close();
        std::cout << "Document created successfully." << std::endl;
    }
    else
    {
        std::cout << "Unable to create the document." << std::endl;
    }
}

// Function to replace an existing parameter value
void replaceParameterValue(ProjectInfo& project, const std::string& parameterName, const std::string& value)
{
    if (parameterName == "Project Name") {
        project.projectName = value;
    }
    else if (parameterName == "Sampling Rate") {
        project.samplingRate = value;
    }
    else if (parameterName == "Input Channels") {
        project.inputChannels = value;
    }
    else if (parameterName == "Output Channels") {
        project.outputChannels = value;
    }
    else
    {
        // Check if the parameter is one of the channel names
        for (int i = 0; i < 8; ++i)
        {
            std::string channelName = "Channel_" + std::to_string(i + 1) + "[";
            if (parameterName.find(channelName) == 0) {
                project.channelNames[i] = value;
                break;
            }
        }
    }
}

void createSessionData()
{
    // Create a sample project
    ProjectInfo project;
    project.projectName = "My Project";
    project.samplingRate = "44100 Hz";
    project.inputChannels = "2";
    project.outputChannels = "2";
    project.channelNames[0] = "Channel1";
    project.channelNames[1] = "Channel2";
    project.channelLocations[0] = "C:\\Users\\alcin\\Coding\\C+++\\document\\";
    project.channelLengths[0] = "0.30.00";
    // ... Initialize the rest of the project data

    // Specify the path where the text file will be written
    std::string filename = "filename.txt";
    std::string filePath = "C:\\Users\\alcin\\Coding\\C+++\\document\\" + filename;

    // Create the document
    createDocument(filePath, project);

    // Replace a parameter value
  //  replaceParameterValue(project, "Project Name", "New Project Name1");



    // Create the document again with updated parameter
   // createDocument(filePath, project);

}