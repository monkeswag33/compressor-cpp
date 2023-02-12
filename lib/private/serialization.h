#pragma once
#include <fstream>
#include "types.h"

void serialize_tree(Node*, std::ofstream&);
void serialize_text(bitpair&, std::ifstream&, unsigned long, std::ofstream&, std::vector<unsigned char>&);
