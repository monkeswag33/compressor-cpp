#include <cstring>
#include <iostream>
#include <bitset>
#include "constants.h"
#include "serialization.h"

inline void serialize_common_node(Node* in, char* out) {
    memcpy(out + NODE_TYPE_OFFSET, &(in->type), NODE_TYPE_SIZE);
}

void serialize_leaf_node(Node* in, char* out) {
    serialize_common_node(in, out);
    memcpy(out + CHAR_OFFSET, &(in->chr), CHAR_SIZE);
}

void serialize_tree(Node* root, std::ofstream& file, char* buffer) {
    unsigned int size;
    switch (root->type) {
    case LEAF_NODE:
        size = LEAF_NODE_SIZE;
        serialize_leaf_node(root, buffer);
        break;

    case PSEUDO_NODE:
    case INTERNAL_NODE:
        size = COMMON_NODE_SIZE;
        serialize_common_node(root, buffer);
        break;
    }
    file.write(buffer, size);
    if (root->type == INTERNAL_NODE) {
        serialize_tree(root->left, file, buffer);
        serialize_tree(root->right, file, buffer);
    }
}

void serialize_tree(Node* root, std::ofstream& file) {
    char* biggest_node = new char[LEAF_NODE_SIZE];
    serialize_tree(root, file, biggest_node);
    delete[] biggest_node;
}

void write_bit(unsigned char bit, unsigned char& bit_count, unsigned char& byte, unsigned int& num_bytes, std::ofstream& ptr) {
    byte ^= (-bit ^ byte) &  (1UL << bit_count);
    (bit_count)++;
    if (bit_count == BITS_PER_BYTE) {
        ptr << byte;
        (num_bytes)++;
        byte = 0;
        bit_count = 0;
    }
}

void write_bytes(char* buffer, unsigned int bufsize, bitpair& bp, unsigned char& bit_count, unsigned char& int_byte, unsigned int& num_bytes, std::ofstream& ptr) {
    for (unsigned int s = 0; s < bufsize; s++) {
        char c = buffer[s];
        std::vector<unsigned char> bits = bp[c];
        for (unsigned char bit : bits)
            write_bit(bit, bit_count, int_byte, num_bytes, ptr);
    }
}

void serialize_text(bitpair& bp, std::string filename, long fsize, std::ofstream& ptr, std::vector<unsigned char>& pseudo_bits) {
    unsigned int num_bytes = 0;
    unsigned char int_byte = 0; // Intermediate byte
    unsigned char bit_count = 0;
    unsigned int full_chunks = fsize / BUFFER_SIZE;
    unsigned int leftover_bytes = fsize % BUFFER_SIZE;
    char* buffer = new char[BUFFER_SIZE];
    FILE* source = fopen(filename.c_str(), "rb");
    fseek(source, 0, SEEK_SET);
    long num_bytes_offset = ptr.tellp();
    ptr.seekp(sizeof(unsigned int), std::ios::cur);
    for (int i = 0; i < full_chunks; i++) {
        fread(buffer, sizeof(char), BUFFER_SIZE, source);
        write_bytes(buffer, BUFFER_SIZE, bp, bit_count, int_byte, num_bytes, ptr);
    }
    if (leftover_bytes) {
        fread(buffer, sizeof(char), leftover_bytes, source);
        write_bytes(buffer, leftover_bytes, bp, bit_count, int_byte, num_bytes, ptr);
    }
    delete[] buffer;
    fclose(source);
    if (bit_count) {
        while (bit_count) {
            for (unsigned char bit : pseudo_bits) {
                write_bit(bit, bit_count, int_byte, num_bytes, ptr);
                if (!bit_count)
                    break;
            }
        }
    }
    ptr.seekp(num_bytes_offset, std::ios::beg);
    ptr << num_bytes;
}