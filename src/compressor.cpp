#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "compression.h"
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass directory to compress\n";
        return 1;
    }
    std::string directory = argv[1];
    while (directory.back() == '/')
        directory.pop_back();
    const fs::path path(directory);
    if (!fs::exists(path)) {
        std::cout << "Path does not exist\n";
        return 1;
    }
    std::string out_file = path.stem().string() + ".cmp";
    std::ofstream file(out_file, std::ios::binary | std::ios::trunc);
    bool is_dir = fs::is_directory(path);
    file.write(reinterpret_cast<char*>(&is_dir), 1);
    if (is_dir)
        compress_dir(file, path);
    else
        compress_file(file, path);
    file.close();
    return 0;
}