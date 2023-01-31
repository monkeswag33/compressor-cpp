#include <string>
#include <iostream>
#include <bitset>
#include <fstream>
#include <filesystem>
#include "constants.h"
#include "types.h"
#include "util.h"
namespace fs = std::filesystem;

void read_node(std::ifstream& file, Node* node) {
    unsigned char node_type;
    file.read(reinterpret_cast<char*>(&node_type), sizeof(node_type));
    node->type = static_cast<NodeType>(node_type);
    if (node->type == LEAF_NODE)
        file.read(&(node->chr), sizeof(node->chr));
}

Node* read_tree(std::ifstream& file) {
    Node* root = new Node;
    read_node(file, root);
    if (root->type == INTERNAL_NODE) {
        root->left = read_tree(file);
        root->right = read_tree(file);
    }
    return root;
}

unsigned char get_bit(const char c, unsigned int& byte_count, unsigned char& bit_count) {
    unsigned char bit = (c >> bit_count) & 1;
    (bit_count)++;
    if (bit_count == BITS_PER_BYTE) {
        bit_count = 0;
        byte_count += 1;
    }
    return bit;
}

void read_file_contents(std::ifstream& file, std::ofstream& outfile, const unsigned int num_bytes, Node* const root) {
    Node* cur_node = root;
    unsigned int byte_count = 0;
    unsigned char bit_count = 0;
    char c;
    for (int i = 0; i < num_bytes; i++) {
        file >> c;
        for (int i = 0; i < BITS_PER_BYTE; i++) {
            unsigned char bit = get_bit(c, byte_count, bit_count);
            if (bit) {
                cur_node = cur_node->right;
            } else {
                cur_node = cur_node->left;
            }
            if (cur_node->type == LEAF_NODE) {
                outfile.write(&(cur_node->chr), sizeof(cur_node->chr));
            }
            if (cur_node->type != INTERNAL_NODE)
                cur_node = root;
        }
    }
}

void decompress_file(std::ifstream& file, std::string out_dir) {
    std::string out_file;
    std::getline(file, out_file, '\0');
    std::ofstream outfile(out_dir + '/' + out_file, std::ios::binary | std::ios::trunc);
    Node* root = read_tree(file);
    unsigned int num_bytes;
    file.read(reinterpret_cast<char*>(&num_bytes), sizeof(num_bytes));
    read_file_contents(file, outfile, num_bytes, root);
    outfile.close();
    free_tree(root);
}

void decompress_dir(std::ifstream& file) {
    std::string out_dir;
    std::getline(file, out_dir, '\0');
    fs::create_directory(out_dir);
    unsigned int num_files;
    file.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
    for (int i = 0; i < num_files; i++)
        decompress_file(file, out_dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please pass the filename to decompress\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename, std::ios::binary);
    bool is_dir;
    file.read(reinterpret_cast<char*>(&is_dir), sizeof(is_dir));
    if (is_dir)
        decompress_dir(file);
    else
        decompress_file(file, ".");
    file.close();
}