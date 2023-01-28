#include <iostream>
#include <filesystem>
#include <fstream>
#include "compression.h"
#include "constants.h"
#include "util.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass directory to compress" << std::endl;
        return 1;
    }
    std::string directory = argv[1];
    while (directory.back() == '/')
        directory.pop_back();
    std::string base_dir = directory.substr(directory.find_last_of("/\\") + 1);
    std::string out_file = base_dir + ".cmp";
    std::ofstream file(out_file, std::ios::binary | std::ios::trunc);
    write_string(base_dir, file);
    unsigned int offset = file.tellp();
    file.seekp(sizeof(unsigned int), std::ios::cur);
    unsigned int num_files = 0;
    for (const auto& entry : fs::directory_iterator(directory)) {
        compress_file(file, entry.path());
        num_files++;
    }
    file.seekp(offset);
    file.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));
    file.close();
    return 0;
}