#include <fstream>
#include <filesystem>
#include <iostream>
#include "util.h"
#include "compression.h"
namespace fs = std::filesystem;

int main() {
    const fs::path in_path("OneFile.txt");
    const fs::path out_path("OneFile.cmp");
    const fs::path decompressed_path("OneFile.txt.dcmp");
    compress_file(in_path);
    decompress_file(out_path, decompressed_path);
    return !compare_files(in_path, decompressed_path);
}