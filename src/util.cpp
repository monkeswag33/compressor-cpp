#include "util.h"
#include <iostream>

void print_bitpairs(const bitpair& bp) {
    for (const auto& elem : bp) {
        std::cout << '\'' << elem.first << '\'' << ": ";
        for (const auto& e : elem.second)
            std::cout << +e;
        std::cout << '\n';
    }
}

void printBT(const std::string& prefix, const Node* node, bool isLeft)
{
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    switch (node->type) {
    case LEAF_NODE:
        std::cout << '\'' << node->chr << "'";
        break;
    case PSEUDO_NODE:
        std::cout << " PSEUDO NODE";
        break;
    }
    std::cout << std::endl;
    if (node->type == INTERNAL_NODE) {
        printBT( prefix + (isLeft ? "│  " : "   "), node->left, true);
        printBT( prefix + (isLeft ? "│  " : "   "), node->right, false);
    }
}

void printBT(const Node& node)
{
    printBT("", &node, false);    
}

void free_tree(const Node* root) {
    if (root->type == INTERNAL_NODE) {
        free_tree(root->left);
        free_tree(root->right);
    }
    delete root;
}
