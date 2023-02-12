#include <filesystem>
#include <iostream>
#include "compression.h"
#include "util.h"
namespace fs = std::filesystem;

int main() {
    const fs::path directory("directory/");
    const fs::path compressed_path("directory.cmp");
    const fs::path decompressed_path("directory1/");
    compress_dir(directory, compressed_path);
    decompress_dir(compressed_path, decompressed_path);
    fs::path out_file;
    for (const fs::path& src_file : fs::directory_iterator(directory)) {
        out_file = decompressed_path / src_file.filename();
        if (!fs::exists(out_file) || !compare_files(src_file, out_file))
            return 1;
    }
}
