#pragma once
#include <cstdio>
#include <string>
#include "types.h"

void serialize_tree(Node*, FILE*, char*);
void serialize_tree(Node*, FILE*);
void serialize_text(bitpair&, std::string, long, FILE*, std::vector<unsigned char>&);
