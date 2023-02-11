#include <fstream>
#include <filesystem>
#include <iostream>
#include "compression.h"

int main() {
    std::filesystem::path in_path("OneFile.txt");
    std::filesystem::path out_path("OneFile.cmp");
    std::string decompressed_path("OneFile.txt.dcmp");
    compress_file(in_path);
    decompress_file(out_path, decompressed_path);
    std::ifstream in_file(in_path, std::ios::binary | std::ios::ate);
    std::ifstream out_file(decompressed_path, std::ios::binary | std::ios::ate);
    if (in_file.tellg() != out_file.tellg())
        return 1;
    in_file.seekg(0);
    out_file.seekg(0);
    bool equal = std::equal(std::istreambuf_iterator<char>(in_file),
                            std::istreambuf_iterator<char>(),
                            std::istreambuf_iterator<char>(out_file));
    in_file.close();
    out_file.close();
    return !equal;
}