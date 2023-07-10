#include <iostream>
#include <fstream>
#include <string>

// Struct to hold project information
struct ProjectInfo {
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
void appendParameter(std::ofstream& file, const std::string& parameterName, const std::string& value) {
    file << parameterName << ": " << value << "\n";
}

// Function to create the document
std::string createDocument(const std::string& path, const ProjectInfo& project) {
    std::ofstream file(path);

    if (file.is_open()) {
        appendParameter(file, "Project Name", project.projectName);
        appendParameter(file, "Sampling Rate", project.samplingRate);
        appendParameter(file, "Input Channels", project.inputChannels);
        appendParameter(file, "Output Channels", project.outputChannels);

        for (int i = 0; i < 8; ++i) {
            std::string channelInfo = "Channel_" + std::to_string(i + 1) + "[" +
                project.channelNames[i] + "_" +
                project.channelLocations[i] + "_" +
                project.channelLengths[i] + "_" +
                project.channelStartTimes[i] + "]";
            appendParameter(file, channelInfo, "");
        }

        file.close();
        std::cout << "Document created successfully." << std::endl;
        return path;
    }
    else {
        std::cout << "Unable to create the document." << std::endl;
        return "";
    }
}

// Function to parse the project information from the text file
bool parseProjectInfo(const std::string& filePath, ProjectInfo& project)
{
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("Project Name:") == 0)
                project.projectName = line.substr(line.find(":") + 2);
            else if (line.find("Sampling Rate:") == 0)
                project.samplingRate = line.substr(line.find(":") + 2);
            else if (line.find("Input Channels:") == 0)
                project.inputChannels = line.substr(line.find(":") + 2);
            else if (line.find("Output Channels:") == 0)
                project.outputChannels = line.substr(line.find(":") + 2);
            else {
                for (int i = 0; i < 8; ++i) {
                    std::string channelInfo = "Channel_" + std::to_string(i + 1) + "[";
                    if (line.find(channelInfo) == 0) {
                        std::string channelValue = line.substr(channelInfo.length());
                        project.channelNames[i] = channelValue.substr(0, channelValue.find("_"));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelLocations[i] = channelValue.substr(0, channelValue.find("_"));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelLengths[i] = channelValue.substr(0, channelValue.find("_"));
                        channelValue = channelValue.substr(channelValue.find("_") + 1);
                        project.channelStartTimes[i] = channelValue.substr(0, channelValue.find("]"));
                        break;
                    }
                }
            }
        }
        file.close();
        return true;
    }
    else {
        std::cout << "Unable to open the file." << std::endl;
        return false;
    }
}

int main() {
    ProjectInfo project;
    project.projectName = "My Project";
    project.samplingRate = "44800";
    project.inputChannels = "2";
    project.outputChannels = "2";
    project.channelLocations[0] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel1.wav";
    project.channelLocations[1] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel2.wav";
    project.channelLocations[2] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel3.wav";
    project.channelLocations[3] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel4.wav";
    project.channelLocations[4] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel5.wav";
    project.channelLocations[5] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel6.wav";
    project.channelLocations[6] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\Channel7.wav";
    project.channelLocations[7] = "C:\\Users\\alcin\\Desktop\\CosmoTest\\Processed\\Channel8.wav";
    project.channelStartTimes[0] = "00_00_00";
    project.channelStartTimes[1] = "00_00_00";
    project.channelStartTimes[2] = "00_00_00";
    project.channelStartTimes[3] = "00_00_00";
    project.channelStartTimes[4] = "00_00_00";
    project.channelStartTimes[5] = "00_00_00";
    project.channelStartTimes[6] = "00_00_00";
    project.channelStartTimes[7] = "00_00_00";
    project.channelLengths[0] = "00_10_10";
    project.channelLengths[1] = "00_10_10";
    project.channelLengths[2] = "00_10_10";
    project.channelLengths[3] = "00_10_10";
    project.channelLengths[4] = "00_10_10";
    project.channelLengths[5] = "00_10_10";
    project.channelLengths[6] = "00_10_10";
    project.channelLengths[7] = "00_10_10";

    // Initialize other values as needed

    std::string documentPath = "C:\\Users\\alcin\\Desktop\\CosmoTest\\RAW\\name.txt";
    std::string loadedDocumentPath = createDocument(documentPath, project);

    if (parseProjectInfo(loadedDocumentPath, project)) {
        std::cout << "Project Name: " << project.projectName << std::endl;
        std::cout << "Sampling Rate: " << project.samplingRate << std::endl;
        std::cout << "Input Channels: " << project.inputChannels << std::endl;
        std::cout << "Output Channels: " << project.outputChannels << std::endl;

        for (int i = 0; i < 8; ++i) {
            std::cout << "Channel_" << (i + 1) << "[" << project.channelNames[i]
                << "_" << project.channelLocations[i] << "_"
                << project.channelLengths[i] << "_"
                << project.channelStartTimes[i] << "]" << std::endl;
        }
    }
    else {
        std::cout << "Unable to parse the file." << std::endl;
    }


    project.projectName = "My big dick";
    project.samplingRate = "44800";
    project.inputChannels = "2";
    project.outputChannels = "2";
    project.channelLocations[0] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel1.wav";
    project.channelLocations[1] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel2.wav";
    project.channelLocations[2] = "C:\\Users\\alcin\\Desktop\CosmoTest\Processed\Channel3.wav";
    project.channelLocations[3] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel4.wav";
    project.channelLocations[4] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel5.wav";
    project.channelLocations[5] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel6.wav";
    project.channelLocations[6] = "C:\\Users\\alcin\\Desktop\CosmoTest\RAW\Channel7.wav";
    project.channelLocations[7] = "C:\\Users\\alcin\\Desktop\CosmoTest\Processed\Channel8.wav";
    project.channelStartTimes[0] = "00_00_00";
    project.channelStartTimes[1] = "00_00_00";
    project.channelStartTimes[2] = "00_10_00";
    project.channelStartTimes[3] = "00_00_00";
    project.channelStartTimes[4] = "00_00_00";
    project.channelStartTimes[5] = "00_00_00";
    project.channelStartTimes[6] = "00_00_00";
    project.channelStartTimes[7] = "00_00_00";
    project.channelLengths[0] = "00_10_20";
    project.channelLengths[1] = "00_10_20";
    project.channelLengths[2] = "00_10_10";
    project.channelLengths[3] = "00_10_10";
    project.channelLengths[4] = "00_12_10";
    project.channelLengths[5] = "00_10_10";
    project.channelLengths[6] = "00_22_10";
    project.channelLengths[7] = "00_22_10";

    loadedDocumentPath = createDocument(documentPath, project);

    if (parseProjectInfo(loadedDocumentPath, project)) {
        std::cout << "Project Name: " << project.projectName << std::endl;
        std::cout << "Sampling Rate: " << project.samplingRate << std::endl;
        std::cout << "Input Channels: " << project.inputChannels << std::endl;
        std::cout << "Output Channels: " << project.outputChannels << std::endl;

        for (int i = 0; i < 8; ++i) {
            std::cout << "Channel_" << (i + 1) << "[" << project.channelNames[i]
                << "_" << project.channelLocations[i] << "_"
                << project.channelLengths[i] << "_"
                << project.channelStartTimes[i] << "]" << std::endl;
        }
    }
    else {
        std::cout << "Unable to parse the file." << std::endl;
    }

    return 0;
}
