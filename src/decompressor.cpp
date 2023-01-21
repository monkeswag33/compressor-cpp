#include <string>
#include <iostream>
#include "constants.h"
#include "types.h"

void read_node(FILE* file, Node* node) {
    bool node_type;
    fread(&node_type, sizeof(bool), NODE_TYPE_SIZE, file);
    node->leaf = node_type;
    if (node_type) {
        bool pseudo_node;
        fread(&pseudo_node, sizeof(bool), PSEUDO_NODE_SIZE, file);
        node->pseudo = pseudo_node;
        if (!pseudo_node) {
            char c;
            fread(&c, sizeof(char), CHAR_SIZE, file);
            node->chr = c;
        }
    }
}

Node* read_tree(FILE* file, char* buffer) {
    Node* root = new Node;
    read_node(file, root);
    if (!root->leaf) {
        root->left = read_tree(file, buffer);
        root->right = read_tree(file, buffer);
    }
    return root;
}

int main() {
    std::string filename = "out.bin";
    FILE* file = fopen(filename.c_str(), "rb");
    unsigned short num_nodes;
    fread(&num_nodes, NUM_NODES_SIZE, 1, file);
    char* buffer = new char[LEAF_NODE_SIZE];
    Node* root = read_tree(file, buffer);
    delete[] buffer;
    fclose(file);
}