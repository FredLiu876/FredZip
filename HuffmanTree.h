#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H
#include "node.h"
#include <string>
#include <map>

using namespace std;

// Huffman tree
class Tree {

    public:
        Node *head;

        Tree();
        Tree(Node* newHead);
        ~Tree();

        // for testing, otherwise unused method
        void print();
        string encode();
        map<char,string> generateCodes();

    private:
        map<char,string> codes;
        // for testing, otherwise unused method
        void print(Node *startPoint, string code);
        string encode(Node *startPoint);
        void findCode(Node *startPoint, string code);
};

#endif