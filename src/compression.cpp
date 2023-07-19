#include <filesystem>
#include <set>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "types.h"
#include "util.h"
#include "serialization.h"
namespace fs = std::filesystem;

/*
Common node format:
ID [unsigned int]
Node Type [bool]

Internal node format:
<Common node format>
Left node id [unsigned int]
Right node id [unsigned int]

Leaf node format:
<Common node format>
Character [char]
*/

Node* generate_tree(nodepq pq) {
    Node t;
    while (pq.size() > 1) {
        t = pq.top();
        Node *a = new Node;
        *a = t;
        pq.pop();
        t = pq.top();
        Node *b = new Node;
        *b = t;
        pq.pop();
        Node temp;
        temp.type = INTERNAL_NODE;
        temp.left = a;
        temp.right = b;
        
        temp.frequency = a->frequency + b->frequency;
        pq.push(temp);
    }
    t = pq.top();
    Node* root = new Node;
    *root = t;
    return root;
}

void gen_bitpair(Node& node, Node* parent, bitpair& bp, std::vector<unsigned char>& cur_bits, std::vector<unsigned char>& pseudo_bits) {
    switch (node.type) {
    case INTERNAL_NODE:
    {
        std::vector<unsigned char> a = cur_bits;
        a.push_back(0);
        gen_bitpair(*node.left, &node, bp, a, pseudo_bits);
        cur_bits.push_back(1);
        gen_bitpair(*node.right, &node, bp, cur_bits, pseudo_bits);
        break;
    }
    case PSEUDO_NODE:
        if (cur_bits.size() > 8) {
            parent->type = LEAF_NODE;
            if (parent->right == &node) {
                parent->chr = parent->left->chr;
                parent->frequency = parent->left->frequency;
            } else {
                parent->chr = parent->right->chr;
                parent->frequency = parent->right->frequency;
            }
            cur_bits.pop_back();
            bp[parent->chr] = cur_bits;
            delete parent->left;
            delete parent->right;
        }
        pseudo_bits = cur_bits;
        break;
    case LEAF_NODE:
        bp[node.chr] = cur_bits;
        break;
    }
}

void gen_bitpair(Node* node, bitpair& bp, std::vector<unsigned char>& pseudo_bits) {
    std::vector<unsigned char> temp;
    gen_bitpair(*node, nullptr, bp, temp, pseudo_bits);
}

void read_file(std::ifstream& file, long size, nodepq* pq) {
    std::map<char, unsigned int> charpair;
    file.seekg(0);
    char c;
    for (int i = 0; i < size; i++) {
        file.read(&c, sizeof(c));
        charpair[c]++;
    }
    for (const auto& elem : charpair)
        pq->push({ .type = LEAF_NODE, .frequency = elem.second, .chr = elem.first });
}

void compress_file(std::ofstream& file, const fs::path filename) {
    nodepq pq;
    write_string(filename.filename(), file);
    std::ifstream source(filename, std::ios::binary | std::ios::ate);
    unsigned long size = source.tellg();
    read_file(source, size, &pq);
    pq.push({ .type = PSEUDO_NODE, .frequency = 0 });
    Node* root = generate_tree(pq);
    bitpair bp;
    std::vector<unsigned char> pseudo_bits;
    gen_bitpair(root, bp, pseudo_bits);
    serialize_tree(root, file);
    serialize_text(bp, source, size, file, pseudo_bits);
    file.seekp(0, std::ios::end);
    free_tree(root);
}

void compress_file(const fs::path filename, std::string out_file) {
    std::ofstream file(out_file == "" ? filename.stem().replace_extension(".cmp").string() : out_file, std::ios::binary);
    char tmp = 0;
    file.write(&tmp, 1);
    compress_file(file, filename);
    file.close();
}

void compress_dir(std::ofstream& file, const fs::path path) {
    write_string(path.filename(), file);
    unsigned int offset = file.tellp();
    file.seekp(sizeof(unsigned int), std::ios::cur);
    unsigned int num_files = 0;
    for (const auto& entry : fs::directory_iterator(path)) {
        compress_file(file, entry.path());
        num_files++;
    }
    file.seekp(offset);
    file.write(reinterpret_cast<char*>(&num_files), sizeof(num_files));
}

void compress_dir(const fs::path path, std::string out_file) {
    std::ofstream file(out_file == "" ? path.stem().append(".cmp").string() : out_file, std::ios::binary);
    char tmp = 1;
    file.write(&tmp, 1);
    compress_dir(file, path);
    file.close();
}
