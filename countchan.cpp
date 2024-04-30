#include <libopenmpt.hpp>
#include <libopenmpt_ext.hpp>
#include <vector>
#include <fstream>

std::shared_ptr<openmpt::module> loadModuleFromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::vector<char> fileData((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());

    auto *mod = new openmpt::module(fileData);
    return std::shared_ptr<openmpt::module>(mod);
}

std::size_t framesRendered = 0;

int getChannelCount(const std::shared_ptr<openmpt::module>& mod) {
    return mod->get_current_playing_channels();
}

int getActiveChannelCount(const std::shared_ptr<openmpt::module>& mod, int sampleRate, std::size_t bufferSize) {
    std::vector<int16_t> leftBuffer(bufferSize);
    std::vector<int16_t> rightBuffer(bufferSize);

    // Render a block of audio
    framesRendered = mod->read(sampleRate, bufferSize, leftBuffer.data(), rightBuffer.data());

    // Check if any channels are active
    int maxChannels = 0;

    for (std::size_t i = 0; i < framesRendered; ++i) {
        if (leftBuffer[i] != 0 || rightBuffer[i] != 0) {
            maxChannels = std::max(getChannelCount(mod), maxChannels);
            break;
        }
    }

    return maxChannels;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; // Exit with error code
    }

    std::string filename(argv[1]);
    std::cout << "Scanning: " << filename << std::endl;
    auto mod = loadModuleFromFile(filename);
    int sampleRate = 48000;
    std::size_t bufferSize = 480; // e.g. 10ms buffer at 48kHz

    int maxChannels = 0;
    bool songFinished = false;
    while (!songFinished) {
        maxChannels = std::max(maxChannels, getActiveChannelCount(mod, sampleRate, bufferSize));
        // Check if the end of the song has been reached
        songFinished = (framesRendered == 0);
    }
    if (maxChannels > 12) {
        std::cout << "Max channels: " << maxChannels << ". This song WILL fail on GBA!" << std::endl;
        return 1;
    }
    else if (maxChannels > 8) {
        std::cout << "Max channels: " << maxChannels << ". This song may struggle on GBA when SFX play!" << std::endl;
    }
    else {
        // The less I know the better
        //std::cout << "Song fine for GBA! (max channels: " << maxChannels << ")" << std::endl;
    }
    return 0;
}
