#ifndef NODE_H
#define NODE_H

#include <string>
using namespace std;

// Node for huffman tree
struct Node {

    Node *left, *right;
    string characters;
    unsigned int freq;

    Node();
    ~Node();

    void set(string newCharacter, unsigned int newFreq, Node *newLeft, Node *newRight);
    void set(string newCharacter, Node *newLeft, Node *newRight);
    void set(string newCharacter, unsigned int newFreq);

    // for testing, otherwise unused method
    void print();

    // for testing, otherwise unused method
    void print(string code);
    
};

#endif