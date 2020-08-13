#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <bitset>

using namespace std;

struct Node {

    Node *left, *right;
    string characters;
    unsigned int height;
    unsigned int freq;

    void set(string newCharacters, unsigned int newFreq, Node *newLeft, Node *newRight, unsigned int newHeight) {
        characters = newCharacters;
        freq = newFreq;
        left = newLeft;
        right = newRight;
        height = newHeight;
    }

    void print() {
        cout << characters << " -> " << freq << '\n';
    }

    void print(string code) {
        cout << characters << " -> " << code << '\n';
    }
    
};

struct Tree {

    Node *head;
    map<char,string> codes;

    void print(Node *startPoint, string code) {

        if (startPoint->left != nullptr) {
            print(startPoint->left, code + "0");
            print(startPoint->right, code + "1");
        } else {
            startPoint->print(code);
        }

    }

    string encode(Node *startPoint) {

        string returnString;
        if (startPoint->left != nullptr) {
            returnString = encode(startPoint->left);
            returnString += encode(startPoint->right) + "0";
        } else {
            returnString = "1" + startPoint->characters;
        }
        return returnString;

    }

    void findCode(Node *startPoint, string code) {

        if (startPoint->left != nullptr) {
            findCode(startPoint->left, code + "0");
            findCode(startPoint->right, code + "1");
        } else {
            codes.insert(pair<char,string>(startPoint->characters[0],code));
        }

    }

    void generateCodes() {
        findCode(head, "");
    }
};

int main(int argc, char *argv[]) {

    string target = argv[1];
    ifstream ifs(target);
    string encode ((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

    map<char,int> frequencies;
    for (unsigned int i = 0; i < encode.length(); i++) {
        
        map<char,int>::iterator index = frequencies.find(encode[i]);
        if (index == frequencies.end()) {
            frequencies.insert(pair<char,int>(encode[i], 1));
        } else {
            index->second += 1;
        }

    }

    map<string,Node*> combined;
    for (auto i : frequencies) {

        Node *copy = new Node;
        copy->set(string(1, i.first), i.second, nullptr, nullptr, 1);
        combined.insert(pair<string,Node*>(string(1, i.first), copy));

    }

    Tree huffman;
    for (unsigned int index = 0; index < frequencies.size()-1; index++) {
        
        Node *least = nullptr;
        Node *least2 = nullptr;
        for (auto i : combined) {

            if (least == nullptr) {
                least = i.second;
            } else if (least2 == nullptr) {
                if (i.second->freq < least->freq) {
                    least2 = least;
                    least = i.second;
                } else {
                    least2 = i.second;
                }
            } else {
                if (i.second->freq < least->freq) {
                    least2 = least;
                    least = i.second;
                } else if (i.second->freq < least2->freq) {
                    least2 = i.second;
                }
            }

        }

        Node *superNode = new Node;
        if (least->height < least2->height) {
            superNode->set(least->characters+least2->characters, least->freq+least2->freq, least, least2, least2->height+1);
        } else if (least->height == least2->height) {
            if (least->characters[0] < least2->characters[0]) {
                superNode->set(least->characters+least2->characters, least->freq+least2->freq, least, least2, least2->height+1);
            } else {
                superNode->set(least->characters+least2->characters, least->freq+least2->freq, least2, least, least2->height+1);
            }
        } else {
            superNode->set(least->characters+least2->characters, least->freq+least2->freq, least2, least, least->height+1);
        }
        
        combined.erase(least->characters);
        combined.erase(least2->characters);
        combined.insert(pair<string,Node*>(superNode->characters, superNode));

        if (index == frequencies.size()-2) {
            huffman = {superNode};
        }

    }

    huffman.generateCodes();
    huffman.print(huffman.head, "");

    string newFilename;
    for (unsigned char i = 0; i < target.length(); i++) {
        if (target[i] == '.') {
            newFilename = target.substr(0,i) + ".zip";
        }
    }
    ofstream newFile (newFilename);

    newFile << encode.length() << "-";
    newFile << huffman.codes.size();
    newFile << huffman.encode(huffman.head);

    string bits = "";
    for (unsigned int i = 0; i < encode.length(); i++) {
        bits += huffman.codes.at(encode[i]);
    }
    if (bits.length()%8 != 0) {
        for (unsigned int i = 0; i < 8-(bits.length()%8); i++) {
            bits += "0";
        }
    }

    cout << bits << endl;

    for (unsigned int i = 0; i < bits.length()/8; i++) {
        bitset<8> toChar(bits.substr(8*i,8*(i+1)));
        cout << char(toChar.to_ulong());
        newFile << char(toChar.to_ulong());
    }

    cout << '\n' << "here";

    newFile.close();
}