#include "vega_file.hpp"

namespace vega {
    std::string VEGAReadFile(const char* file_path) {
        std::ifstream file(file_path, std::ios::binary);
        std::string contents;
        
        file.seekg(0, std::ios::end);
        contents.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&contents[0], contents.size());
        file.close();

        return contents;
    }
}