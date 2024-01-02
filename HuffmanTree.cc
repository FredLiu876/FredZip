#include <bitset>
#include <map>
#include <string>
#include "HuffmanTree.h"
#include "Node.h"
#include "SubtreeStack.h"

using namespace std;

Tree::Tree() {
    head = nullptr;
}

Tree::Tree(Node* newHead) {
    head = newHead;
}

Tree::Tree(string encode) {
    // build map of character frequencies in the string, each character frequency represented as a node
    map<string,Node*> frequencies;
    for (unsigned int i = 0; i < encode.length(); i++) {
        string curLetter = string(1, encode[i]);
        auto it = frequencies.find(curLetter);
        if (it == frequencies.end()) {
            Node *node = new Node;
            node->set(curLetter, 1, nullptr, nullptr);
            frequencies.insert(pair<string,Node*>(curLetter, node));
        } else {
            it->second->freq += 1;
        }
    }

    // combine nodes to build tree
    unsigned int size = frequencies.size();
    for (unsigned int i = 0; i < size-1; i++) {
        Node *least = nullptr;
        Node *least2 = nullptr;
        for (auto it : frequencies) {
            if (least == nullptr) {
                least = it.second;
            } else if (least2 == nullptr) {
                if (it.second->freq < least->freq) {
                    least2 = least;
                    least = it.second;
                } else {
                    least2 = it.second;
                }
            } else {
                if (it.second->freq < least->freq) {
                    least2 = least;
                    least = it.second;
                } else if (it.second->freq < least2->freq) {
                    least2 = it.second;
                }
            }
        }

        Node *superNode = new Node;
        superNode->set(least->characters+least2->characters, least->freq+least2->freq, least, least2);
        
        frequencies.erase(least->characters);
        frequencies.erase(least2->characters);
        frequencies.insert(pair<string,Node*>(superNode->characters, superNode));

        if (i == size-2) {
            head = superNode;
        }
    }
}

Tree::~Tree() {
    delete head;
}

// for testing, otherwise unused method
void Tree::print() {
    print(head, "");
}

string Tree::encodeTreeIntoBits() {
    return encodeTreeIntoBits(head);
}

string Tree::encodeTreeIntoBits(Node *startPoint) {
    string returnString;
    if (startPoint->left != nullptr) {
        returnString = encodeTreeIntoBits(startPoint->left);
        returnString += encodeTreeIntoBits(startPoint->right) + "0";
    } else {
        returnString = "1" + bitset<8>(startPoint->characters[0]).to_string();
    }
    return returnString;
}

map<char,string> Tree::generateLetterCodes() {
    findLetterCode(head, "");
    return codes;
}

void Tree::findLetterCode(Node *startPoint, string code) {
    if (startPoint->left != nullptr) {
        findLetterCode(startPoint->left, code + "0");
        findLetterCode(startPoint->right, code + "1");
    } else {
        codes.insert(pair<char,string>(startPoint->characters[0],code));
    }
}

string Tree::getEncoding(string encode) {
    map<char,string> codes = generateLetterCodes();
    string bits = bitset<32>(encode.length()).to_string();
    bits += bitset<8>(codes.size()).to_string();
    bits += encodeTreeIntoBits();
    for (unsigned int i = 0; i < encode.length(); i++) {
        bits += codes.at(encode[i]);
    }
    return bits;
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

string compressionHuffman(string encode) {
    Tree huffman = Tree(encode);
    string bits = huffman.getEncoding(encode);
    return bits;
}

string decompressionHuffman(string decode) {
    string allBits = "";
    for (unsigned int i = 0; i < decode.length(); i++) {
        allBits += bitset<8>(decode[i]).to_string();
    }

    allBits = allBits.substr(1); // Skip the first bit used to mark compression methods

    unsigned int numChars = bitset<32>(allBits.substr(0,32)).to_ulong();
    unsigned int charsInTree = bitset<8>(allBits.substr(32,8)).to_ulong();

    allBits = allBits.substr(40);

    SubtreeStack subtreeStack;
    unsigned int charsFound = 0;
    unsigned int bitIndex = 0;
    while (charsFound != charsInTree || subtreeStack.getSize() != 1) {
        if (allBits[bitIndex] == '1') {
            Node *leaf = new Node;
            leaf->set(string(1, bitset<8>(allBits.substr(bitIndex+1,8)).to_ulong()), 0, nullptr, nullptr);
            subtreeStack.push(leaf);
            bitIndex += 9;
            charsFound += 1;
        } else {
            subtreeStack.pop();
            bitIndex += 1;
        }
    }

    Tree huffman = {subtreeStack.head->data};
    allBits = allBits.substr(bitIndex);

    string decodedString = "";
    unsigned int charsExtracted = 0;
    Node *selectedNode = huffman.head;
    for (unsigned int i = 0; i < allBits.length(); i++) {
        if (allBits[i] == '0') {
            selectedNode = selectedNode->left; 
        } else {
            selectedNode = selectedNode->right;
        }

        if (selectedNode->left == nullptr) {
            decodedString += selectedNode->characters;
            selectedNode = huffman.head;
            charsExtracted += 1;
        }

        if (charsExtracted == numChars) {
            break;
        }
    }
    return decodedString;
}