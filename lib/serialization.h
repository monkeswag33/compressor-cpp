#pragma once
#include <fstream>
#include <string>
#include "types.h"

void serialize_tree(Node*, std::ofstream&, char*);
void serialize_tree(Node*, std::ofstream&);
void serialize_text(bitpair&, std::ifstream&, unsigned long, std::ofstream&, std::vector<unsigned char>&);
