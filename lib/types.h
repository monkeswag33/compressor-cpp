#pragma once
#include <queue>
#include <map>

enum NodeType : unsigned char {
    INTERNAL_NODE,
    LEAF_NODE,
    PSEUDO_NODE
};

struct Node {
    NodeType type;
    unsigned int frequency;
    Node *left, *right; // Uninitialized if not internal
    char chr; // Uninitialized if not leaf
};

class Compare {
public:
    bool operator() (Node, Node);
};

typedef std::priority_queue<Node, std::vector<Node>, Compare> nodepq;
typedef std::map<char, std::vector<unsigned char>> bitpair;
