#pragma once
#include <queue>
#include <map>

struct Node {
    bool leaf;
    unsigned int frequency;
    bool pseudo; // Pseudo symbol
    Node *left, *right; // Uninitialized if not internal
    char chr; // Uninitialized if not leaf
};

class Compare {
public:
    bool operator() (Node a, Node b);
};

typedef std::priority_queue<Node, std::vector<Node>, Compare> nodepq;
typedef std::map<char, std::vector<unsigned char>> bitpair;
