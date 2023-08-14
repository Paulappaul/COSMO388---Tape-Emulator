#pragma once
#include "Cosmo.h"



void appendParameter(std::ofstream& file, const std::string& parameterName, const std::string& value)
{
    file << parameterName << ": " << value << "\n";
}

void createDocument(const std::string& path, const ProjectInfo& project)
{
    std::ofstream file(path);
    std::cout << "Path is: " << path << std::endl;

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
                std::to_string(project.channelLengths[i]) + "_" +
                std::to_string(project.channelStartTimes[i]) + "]";
            appendParameter(file, channelInfo, "");
        }

        file.close();
        std::cout << path << std::endl;
        std::cout << "Document created successfully." << std::endl;
    }
    else
    {
        std::cout << path << std::endl;
        std::cout << project.projectName << std::endl;
        std::cout << "Unable to create the document." << std::endl;
    }
}

bool parseProjectInfo(const std::string& filePath, ProjectInfo& project)
{
    std::ifstream file(filePath);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("Project Name:") == 0)
                project.projectName = line.substr(line.find(":") + 2);
            else if (line.find("Sampling Rate:") == 0)
                project.samplingRate = line.substr(line.find(":") + 2);
            else if (line.find("Input Channels:") == 0)
                project.inputChannels = line.substr(line.find(":") + 2);
            else if (line.find("Output Channels:") == 0)
                project.outputChannels = line.substr(line.find(":") + 2);
            else
            {
                for (int i = 0; i < 8; ++i)
                {
                    std::string channelInfo = "Channel_" + std::to_string(i + 1) + "[";
                    if (line.find(channelInfo) == 0)
                    {
                        std::string channelValue = line.substr(channelInfo.length());
                        project.channelNames[i] = channelValue.substr(0, channelValue.find("_"));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelLocations[i] = channelValue.substr(0, channelValue.find("_"));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelLengths[i] = std::stoul(channelValue.substr(0, channelValue.find("_")));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelStartTimes[i] = std::stoul(channelValue.substr(0, channelValue.find("]")));
                        break;
                    }
                }
            }
        }
        file.close();
        return true;
    }
    else
    {
        std::cout << "Unable to open the file." << std::endl;
        return false;
    }
}

void createSessionData()
{
    project.projectName = projectName;
    project.samplingRate = std::to_string(sRate);
    project.inputChannels = "2";
    project.outputChannels = "2";
    for (int i = 0; i < 8; ++i)
    {
        project.channelNames[i] = "Channel" + std::to_string(i + 1);
        project.channelLocations[i] = " ";
        project.channelLengths[i] = 0;
        project.channelStartTimes[i] = 0;
    }

    // Specify the path where the text file will be written
    std::string filename = projectName + ".txt";
    std::cout << "Session Data saved to: " << projectPath + filename << std::endl;
    filePath = projectPath + filename;

    // Create the document
    createDocument(filePath, project);
}