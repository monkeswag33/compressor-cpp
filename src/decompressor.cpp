#include <iostream>
#include <fstream>
#include "compression.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass the filename to decompress\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename, std::ios::binary);
    bool is_dir;
    file.read(reinterpret_cast<char*>(&is_dir), sizeof(is_dir));
    if (is_dir)
        decompress_dir(file);
    else
        decompress_file(file, ".", "");
    file.close();
}