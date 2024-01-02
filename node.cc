#include <iostream>
#include <string>
#include "Node.h"

using namespace std;

Node::Node() {
    left = nullptr;
    right = nullptr;
}

Node::~Node() {
    delete left;
    delete right;
}

void Node::set(string newCharacters, unsigned int newFreq, Node *newLeft, Node *newRight) {

    characters = newCharacters;
    freq = newFreq;
    left = newLeft;
    right = newRight;

}

void Node::set(string newCharacters, Node *newLeft, Node *newRight) {

    characters = newCharacters;
    left = newLeft;
    right = newRight;

}

void Node::set(string newCharacters, unsigned int newFreq) {

    characters = newCharacters;
    freq = newFreq;

}

// for testing, otherwise unused method
void Node::print() {
    cout << characters << " -> " << freq << endl;
}

// for testing, otherwise unused method
void Node::print(string code) {
    cout << characters << " -> " << code << '\n';
}