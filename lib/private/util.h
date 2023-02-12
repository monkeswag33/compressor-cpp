#pragma once
#include <string>
#include <fstream>
#include "types.h"

void printBT(const Node&);
void print_bitpairs(const bitpair&);
void free_tree(const Node*);
inline void write_string(const std::string& s, std::ofstream& file) {
    file.write(s.c_str(), s.size());
    file.write("\0", 1);
}