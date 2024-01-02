#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H
#include <map>
#include <string>
#include "Node.h"

using namespace std;

// Huffman tree
class Tree {

    public:
        Node *head;

        Tree();
        Tree(Node* newHead);
        Tree(string encode);
        ~Tree();

        string getEncoding(string encode);
        string encodeTreeIntoBits();
        map<char,string> generateLetterCodes();

        // for testing, otherwise unused method
        void print();

    private:
        map<char,string> codes;
        // for testing, otherwise unused method
        void print(Node *startPoint, string code);
        string encodeTreeIntoBits(Node *startPoint);
        void findLetterCode(Node *startPoint, string code);
};

string compressionHuffman(string encode);
string decompressionHuffman(string decode);

#endif