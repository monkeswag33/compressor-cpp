#include <fstream>
#include "compression.h"
#include "util.h"
#include "types.h"
namespace fs = std::filesystem;

void read_node(std::ifstream& file, Node* node) {
    file.read(reinterpret_cast<char*>(&(node->type)), sizeof(node->type));
    // std::cout << (node->type == INTERNAL_NODE) << '\n';
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

void read_file_contents(std::ifstream& file, std::ofstream& outfile, const unsigned int num_bytes, Node* const root) {
    Node* cur_node = root;
    // std::cout << cur_node->type;
    char c;
    for (int i = 0; i < num_bytes; i++) {
        file.read(&c, sizeof(c));
        for (int j = 0; j < 8; j++) {
            unsigned char bit = (c >> j) & 1;
            if (bit) {
                cur_node = cur_node->right;
            } else {
                cur_node = cur_node->left;
            }
            if (cur_node->type == LEAF_NODE)
                outfile.write(&(cur_node->chr), sizeof(cur_node->chr));
            if (cur_node->type != INTERNAL_NODE)
                cur_node = root;
        }
    }
}

void decompress_file(std::ifstream& file, fs::path out_dir, std::string out_file) {
    std::filesystem::create_directories(out_dir);
    if (out_file == "")
        std::getline(file, out_file, '\0');
    else
        file.ignore(256, '\0');
    std::ofstream outfile(out_dir / out_file, std::ios::binary | std::ios::trunc);
    Node* root = read_tree(file);
    unsigned int num_bytes;
    file.read(reinterpret_cast<char*>(&num_bytes), sizeof(num_bytes));
    read_file_contents(file, outfile, num_bytes, root);
    outfile.close();
    free_tree(root);
}

void decompress_file(fs::path filename, std::string out_file) {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(1);
    decompress_file(file, ".", out_file);
    file.close();
}

void decompress_dir(std::ifstream& file) {
    std::string out_dir;
    std::getline(file, out_dir, '\0');
    fs::create_directory(out_dir);
    unsigned int num_files;
    file.read(reinterpret_cast<char*>(&num_files), sizeof(num_files));
    for (int i = 0; i < num_files; i++)
        decompress_file(file, out_dir, "");
}

void decompress_dir(fs::path& filename) {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(1);
    decompress_dir(file);
    file.close();
}
