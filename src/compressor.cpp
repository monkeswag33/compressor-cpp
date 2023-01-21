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
        temp.leaf = false;
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

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    if (node->leaf) {
        std::cout << " - ";
        if (node->pseudo)
            std::cout << " - PSEUDO NODE";
        else
            std::cout << '\'' << node->chr << "'";
    }
    std::cout << std::endl;
    if (!node->leaf) {
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);    
}

void gen_bitpair(Node* node, Node* parent, bitpair* bp, std::vector<unsigned char>* cur_bits, std::vector<unsigned char>* pseudo_bits) {
    if (!node->leaf) {
        std::vector<unsigned char> a = *cur_bits;
        a.push_back(0);
        gen_bitpair(node->left, node, bp, &a, pseudo_bits);
        cur_bits->push_back(1);
        gen_bitpair(node->right, node, bp, cur_bits, pseudo_bits);
    } else {
        if (!cur_bits->size())
            cur_bits->push_back(0);
        if (node->pseudo) {
            if (cur_bits->size() >= BITS_PER_BYTE) {
                delete parent->left;
                delete parent->right;
                parent->leaf = true;
                parent->chr = parent->right->chr;
                parent->frequency = parent->right->frequency;
                parent->pseudo = false;
                cur_bits->pop_back();
            }
            *pseudo_bits = *cur_bits;
        }
        else 
            (*bp)[node->chr] = *cur_bits;
    }
}

nodepq frequencies(std::string str) {
    std::set<char> unique(str.begin(), str.end());
    std::priority_queue<Node, std::vector<Node>, Compare> pq;
    for (char c : unique) {
        unsigned int frequency = std::count(str.begin(), str.end(), c);
        Node n;
        n.chr = c;
        n.frequency = frequency;
        n.leaf = true;
        n.pseudo = false;
        pq.push(n);
    }
    return pq;
}

void free_tree(Node* root) {
    if (!root->leaf) {
        free_tree(root->left);
        free_tree(root->right);
    }
    delete root;
}

void get_counts(char* buffer, unsigned int size, std::map<char, unsigned int>* charpair) {
    std::set<char> unique(buffer, buffer + size);
    for (char c : unique)
        (*charpair)[c] += std::count(buffer, buffer + size, c);
}

long read_file(std::string filename, nodepq* pq) {
    std::map<char, unsigned int> charpair;
    FILE* file = fopen(filename.c_str(), "rb");
    char* buffer = new char[BUFFER_SIZE];
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    unsigned int full_chunks = size / BUFFER_SIZE;
    for (int i = 0; i < full_chunks; i++) {
        fread(buffer, sizeof(char), BUFFER_SIZE, file);
        get_counts(buffer, BUFFER_SIZE, &charpair);
    }
    unsigned int leftover_bytes = size % BUFFER_SIZE;
    if (leftover_bytes) {
        fread(buffer, sizeof(char), leftover_bytes, file);
        get_counts(buffer, leftover_bytes, &charpair);
    }
    delete[] buffer;
    fclose(file);
    for (const auto& elem : charpair)
        pq->push({ .leaf = true, .frequency = elem.second, .pseudo = false, .chr = elem.first });
    return size;
}

void print_bitpairs(bitpair* bp, std::vector<unsigned char>* pseudo_bits) {
    for (const auto& elem : *bp) {
        std::cout << '\'' << elem.first << '\'' << ": ";
        for (const auto& e : elem.second) {
            std::cout << +e;
        }
        if (elem.second == *pseudo_bits)
            std::cout << " - PSEUDO NODE";
        std::cout << '\n';
    }
}

int main() {
    nodepq pq;
    std::string filename = "../file.txt";
    long size = read_file(filename, &pq);
    pq.push({ .leaf = true, .frequency = 0, .pseudo = true });
    Node* root = generate_tree(pq);
    bitpair bp;
    std::vector<unsigned char> pseudo_bits;
    {
        std::vector<unsigned char> temp;
        gen_bitpair(root, nullptr, &bp, &temp, &pseudo_bits);
    }
    printBT(root);
    print_bitpairs(&bp, &pseudo_bits);
    FILE* file = fopen("out.bin", "wb");
    serialize_tree(root, file);
    std::cout << ftell(file) << std::endl;
    serialize_text(&bp, filename, size, file, &pseudo_bits);
    fclose(file);
    free_tree(root);
}
