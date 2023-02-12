#include <fstream>
#include "util.h"
namespace fs = std::filesystem;

bool compare_files(fs::path file1, fs::path file2) {
    std::ifstream in_file(file1, std::ios::binary | std::ios::ate);
    std::ifstream out_file(file2, std::ios::binary | std::ios::ate);
    if (in_file.tellg() != out_file.tellg())
        return false;
    in_file.seekg(0);
    out_file.seekg(0);
    return std::equal(std::istreambuf_iterator<char>(in_file),
                            std::istreambuf_iterator<char>(),
                            std::istreambuf_iterator<char>(out_file));
}
