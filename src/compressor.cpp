#include <iostream>
#include <filesystem>
#include <fstream>
#include "compression.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass directory to compress" << std::endl;
        return 1;
    }
    std::string directory = argv[1];
    std::string out_file = directory.substr(directory.find_last_of("/\\") + 1) + ".cmp";
    std::ofstream file(out_file, std::ofstream::binary);
    file.seekp(sizeof(unsigned int), std::ios::beg);
    unsigned int num_files = 0;
    for (const auto& entry : fs::directory_iterator(directory)) {
        compress_file(file, entry.path());
        num_files++;
    }
    file.seekp(0);
    file << num_files;
    file.close();
    return 0;
}