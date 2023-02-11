#pragma once
#include <fstream>
#include <filesystem>

void compress_file(std::ofstream&, const std::filesystem::path);
void compress_file(const std::filesystem::path, std::string = "");
void compress_dir(std::ofstream&, const std::filesystem::path);
void compress_dir(const std::filesystem::path);

void decompress_file(std::ifstream&, std::filesystem::path, std::string);
void decompress_file(std::filesystem::path, std::string = "");
void decompress_dir(std::ifstream&);
void decompress_dir(std::filesystem::path);