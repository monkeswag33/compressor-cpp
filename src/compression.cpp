#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <bitset>
#include "constants.h"
#include "types.h"
#include "serialization.h"
#include "compression.h"

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
        if (cur_bits.size() >= BITS_PER_BYTE) {
            delete parent->left;
            delete parent->right;
            parent->type = LEAF_NODE;
            parent->chr = parent->right->chr;
            parent->frequency = parent->right->frequency;
            cur_bits.pop_back();
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

nodepq frequencies(std::string str) {
    std::set<char> unique(str.begin(), str.end());
    std::priority_queue<Node, std::vector<Node>, Compare> pq;
    for (char c : unique) {
        unsigned int frequency = std::count(str.begin(), str.end(), c);
        Node n;
        n.chr = c;
        n.frequency = frequency;
        n.type = LEAF_NODE;
        pq.push(n);
    }
    return pq;
}

void free_tree(Node* root) {
    if (root->type == INTERNAL_NODE) {
        free_tree(root->left);
        free_tree(root->right);
    }
    delete root;
}

void read_file(std::ifstream& file, long size, nodepq* pq) {
    std::map<char, unsigned int> charpair;
    file.seekg(0);
    char c;
    for (int i = 0; i < size; i++) {
        file >> c;
        charpair[c]++;
    }
    for (const auto& elem : charpair)
        pq->push({ .type = LEAF_NODE, .frequency = elem.second, .chr = elem.first });
}

void compress_file(std::ofstream& file, std::string filename) {
    std::string base = filename.substr(filename.find_last_of("/\\") + 1);
    nodepq pq;
    file << base;
    file << '\0';
    std::ifstream source(filename, std::ios::binary | std::ios::ate);
    long size = source.tellg();
    read_file(source, size, &pq);
    pq.push({ .type = PSEUDO_NODE, .frequency = 0 });
    Node* root = generate_tree(pq);
    bitpair bp;
    std::vector<unsigned char> pseudo_bits;
    gen_bitpair(root, bp, pseudo_bits);
    serialize_tree(root, file);
    serialize_text(bp, source, size, file, pseudo_bits);
    free_tree(root);
}
