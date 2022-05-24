#include "node.h"
#include "HuffmanTree.h"
#include <string>
#include <map>
#include <bitset>

using namespace std;

Tree::Tree() {
    head = nullptr;
}

Tree::Tree(Node* newHead) {
    head = newHead;
}

Tree::~Tree() {
    delete head;
}

// for testing, otherwise unused method
void Tree::print() {
    print(head, "");
}

string Tree::encode() {
    return encode(head);
}

map<char,string> Tree::generateCodes() {
    findCode(head, "");
    return codes;
}

// for testing, otherwise unused method
void Tree::print(Node *startPoint, string code) {

    if (startPoint->left != nullptr) {
        print(startPoint->left, code + "0");
        print(startPoint->right, code + "1");
    } else {
        startPoint->print(code);
    }

}

string Tree::encode(Node *startPoint) {

    string returnString;
    if (startPoint->left != nullptr) {
        returnString = encode(startPoint->left);
        returnString += encode(startPoint->right) + "0";
    } else {
        returnString = "1" + bitset<8>(startPoint->characters[0]).to_string();
    }
    return returnString;

}

void Tree::findCode(Node *startPoint, string code) {

    if (startPoint->left != nullptr) {
        findCode(startPoint->left, code + "0");
        findCode(startPoint->right, code + "1");
    } else {
        codes.insert(pair<char,string>(startPoint->characters[0],code));
    }

}