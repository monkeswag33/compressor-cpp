#include <string>
#include <iostream>
#include <bitset>
#include "constants.h"
#include "types.h"

void read_node(FILE* file, Node* node) {
    bool node_type;
    fread(&node_type, sizeof(bool), NODE_TYPE_SIZE, file);
    node->leaf = node_type;
    if (node_type) {
        bool pseudo_node;
        fread(&pseudo_node, sizeof(bool), PSEUDO_SIZE, file);
        node->pseudo = pseudo_node;
        if (!pseudo_node) {
            char c;
            fread(&c, sizeof(char), CHAR_SIZE, file);
            node->chr = c;
        }
    }
}

Node* read_tree(FILE* file) {
    Node* root = new Node;
    read_node(file, root);
    if (!root->leaf) {
        root->left = read_tree(file);
        root->right = read_tree(file);
    }
    return root;
}

unsigned char get_bit(const char* const buffer, unsigned char& byte_count, unsigned char& bit_count) {
    char byte = buffer[byte_count];
    unsigned char bit = (byte >> bit_count) & 1;
    (bit_count)++;
    if (bit_count == BITS_PER_BYTE) {
        bit_count = 0;
        byte_count += 1;
    }
    return bit;
}

void get_chars(const char* const buffer, const unsigned int bufsize, Node* cur_node, Node* const root) {
    unsigned char byte_count = 0, bit_count = 0;
    for (int i = 0; i < bufsize * BITS_PER_BYTE; i++) {
        unsigned char bit = get_bit(buffer, byte_count, bit_count);
        if (bit)
            cur_node = cur_node->right;
        else
            cur_node = cur_node->left;
        if (cur_node->leaf) {
            if (!cur_node->pseudo)
                std::cout << cur_node->chr;
            cur_node = root;
        }
    }
    std::cout << std::endl;
}

void read_file_contents(FILE* file, const unsigned int num_bytes, Node* const root) {
    Node cur_node = *root;
    char* buffer = new char[BUFFER_SIZE];
    unsigned int full_chunks = num_bytes / BUFFER_SIZE;
    unsigned int leftover_bytes = num_bytes % BUFFER_SIZE;
    for (int i = 0; i < full_chunks; i++) {
        fread(buffer, sizeof(char), BUFFER_SIZE, file);
        get_chars(buffer, BUFFER_SIZE, &cur_node, root);
    }
    if (leftover_bytes) {
        fread(buffer, sizeof(char), leftover_bytes, file);
        get_chars(buffer, leftover_bytes, &cur_node, root);
    }
    delete[] buffer;
}

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    if (node->leaf) {
        if (node->pseudo)
            std::cout << " PSEUDO NODE";
        else
            std::cout << '\'' << node->chr << "'";
    }
    std::cout << std::endl;
    if (!node->leaf) {
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│  " : "   "), node->left, true);
        printBT( prefix + (isLeft ? "│  " : "   "), node->right, false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);    
}

int main() {
    std::string filename = "out.bin";
    FILE* file = fopen(filename.c_str(), "rb");
    Node* root = read_tree(file);
    unsigned int num_bytes;
    fread(&num_bytes, sizeof(num_bytes), 1, file);
    read_file_contents(file, num_bytes, root);
    fclose(file);
}