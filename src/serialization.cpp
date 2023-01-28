#include <cstring>
#include <iostream>
#include <bitset>
#include "constants.h"
#include "serialization.h"

void serialize_tree(Node* root, std::ofstream& file) {
    unsigned int size;
    file.write(reinterpret_cast<char*>(&(root->type)), sizeof(root->type));
    if (root->type == LEAF_NODE)
        file.write(&(root->chr), sizeof(root->chr));
    else if (root->type == INTERNAL_NODE) {
        serialize_tree(root->left, file);
        serialize_tree(root->right, file);
    }
}

void write_bit(unsigned char bit, unsigned char& bit_count, unsigned char& byte, unsigned int& num_bytes, std::ofstream& ptr) {
    byte ^= (-bit ^ byte) & (1UL << bit_count);
    if (++bit_count == BITS_PER_BYTE) {
        ptr.write(reinterpret_cast<char*>(&byte), sizeof(byte));
        num_bytes++;
        byte = 0;
        bit_count = 0;
    }
}

void serialize_text(bitpair& bp, std::ifstream& source, unsigned long fsize, std::ofstream& ptr, std::vector<unsigned char>& pseudo_bits) {
    unsigned int num_bytes = 0;
    unsigned char int_byte = 0; // Intermediate byte
    unsigned char bit_count = 0;
    unsigned long num_bytes_offset = ptr.tellp();
    ptr.seekp(sizeof(unsigned int), std::ios::cur);
    source.seekg(0);
    char c;
    for (int i = 0; i < fsize; i++) {
        source.read(&c, sizeof(c));
        std::vector<unsigned char> bits = bp[c];
        for (unsigned char bit : bits)
            write_bit(bit, bit_count, int_byte, num_bytes, ptr);
    }
    source.close();
    if (bit_count) {
        while (bit_count) {
            for (unsigned char bit : pseudo_bits) {
                write_bit(bit, bit_count, int_byte, num_bytes, ptr);
                if (!bit_count)
                    break;
            }
        }
    }
    ptr.seekp(num_bytes_offset);
    ptr.write(reinterpret_cast<char*>(&num_bytes), sizeof(num_bytes));
}