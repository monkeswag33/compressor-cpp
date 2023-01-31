#pragma once
#include <fstream>
#include <filesystem>

void compress_file(std::ofstream&, const std::filesystem::path&);
