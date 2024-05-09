#include <libopenmpt.hpp>
#include <libopenmpt_ext.hpp>
#include <memory>
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
    return mod->get_num_channels();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; // Exit with error code
    }

    std::string filename(argv[1]);
    std::cout << "Scanning: " << filename << std::endl;
    auto mod = loadModuleFromFile(filename);

    int maxChannels = getChannelCount(mod);
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
