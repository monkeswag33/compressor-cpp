#include <iostream>
#include <filesystem>
#include <fstream>
#include "compression.h"
#include "constants.h"
#include "util.h"
namespace fs = std::filesystem;

void compress_dir(const fs::path& path, std::ofstream& file) {
    write_string(path.filename(), file);
    unsigned int offset = file.tellp();
    file.seekp(sizeof(unsigned int), std::ios::cur);
    unsigned int num_files = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        compress_file(file, entry.path());
        num_files++;
    }
    file.seekp(offset);
    file.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass directory to compress" << std::endl;
        return 1;
    }
    std::string directory = argv[1];
    while (directory.back() == '/')
        directory.pop_back();
    const fs::path path(directory);
    std::string out_file = path.stem().string() + ".cmp";
    std::ofstream file(out_file, std::ios::binary | std::ios::trunc);
    bool is_dir = fs::is_directory(path);
    file.write(reinterpret_cast<char*>(&is_dir), sizeof(is_dir));
    if (is_dir)
        compress_dir(path, file);
    else
        compress_file(file, path);
    file.close();
    return 0;
}