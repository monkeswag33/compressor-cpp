#include <string>
#include <iostream>
#include "constants.h"
#include "types.h"

void read_node(FILE* file, Node* node) {
    std::cout << ftell(file) << std::endl;
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
        std::cout << "INTERNAL NODE" << std::endl;
        root->left = read_tree(file);
        root->right = read_tree(file);
    }
    std::cout << "LEAF NODE" << std::endl;
    return root;
}

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    if (node->leaf) {
        if (node->pseudo)
            std::cout << " - PSEUDO NODE";
        else
            std::cout << '\'' << node->chr << "'";
    }
    std::cout << std::endl;
    if (!node->leaf) {
        // enter the next tree level - left and right branch
        printBT( prefix + (isLeft ? "│ " : "  "), node->left, true);
        printBT( prefix + (isLeft ? "│ " : "  "), node->right, false);
    }
}

void printBT(const Node* node)
{
    printBT("", node, false);    
}

int main() {
    std::string filename = "out.bin";
    FILE* file = fopen(filename.c_str(), "rb");
    fseek(file, 0, SEEK_SET);
    //unsigned short num_nodes;
    //fread(&num_nodes, NUM_NODES_SIZE, 1, file);
    Node* root = read_tree(file);
    printBT(root);
    fclose(file);
}