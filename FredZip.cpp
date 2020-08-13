#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <bitset>

using namespace std;

/**************************************************************************************************************
 * Struct and class definitions
**************************************************************************************************************/

struct Node {

    Node *left, *right;
    string characters;
    unsigned int freq;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    void set(string newCharacters, unsigned int newFreq, Node *newLeft, Node *newRight) {

        characters = newCharacters;
        freq = newFreq;
        left = newLeft;
        right = newRight;

    }

    void set(string newCharacters, Node *newLeft, Node *newRight) {

        characters = newCharacters;
        left = newLeft;
        right = newRight;

    }

    void set(string newCharacters, unsigned int newFreq) {

        characters = newCharacters;
        freq = newFreq;

    }

    void print() {
        cout << characters << "(" << 0+characters[0] << ")" << " -> " << freq << '\n';
    }

    void print(string code) {
        cout << characters << " -> " << code << '\n';
    }
    
};

class Tree {

    public:
        Node *head;

        Tree() {
            head = nullptr;
        }

        Tree(Node* newHead) {
            head = newHead;
        }

        void print() {
            print(head, "");
        }

        string encode() {
            return encode(head);
        }

        map<char,string> generateCodes() {
            findCode(head, "");
            return codes;
        }

    private:
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
                returnString = "1" + bitset<8>(startPoint->characters[0]).to_string();
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

};

struct Subtree {
    Node *data;
    Subtree *next;

    Subtree() {
        data = nullptr;
        next = nullptr;
    }
};

class SubtreeStack {

    public:
        Subtree *head;

        SubtreeStack() {
            size = 0;
            head = nullptr;
        }

        int getSize() {
            int returnSize = size;
            return returnSize;
        }

        void push(Node *node) {

            Subtree *newSubtree = new Subtree;
            newSubtree->data = node;
            if (head != nullptr) {
                newSubtree->next = head;
            }
            head = newSubtree;
            size += 1;

        }

        void pop() {

            Node *node1 = head->next->data;
            Node *node2 = head->data;
            Node *combined = new Node;
            combined->set(node1->characters+node2->characters, node1, node2);
            size -= 2;
            if (size == 0) {
                head = nullptr;
            } else {
                head = head->next->next;
            }
            push(combined);

        }

        void print() {
            print(head);
        }

    private:
        unsigned int size;

        void print(Subtree *startPoint) {
            startPoint->data->print();
            if (startPoint->next != nullptr) {
                print(startPoint->next);
            }
        }

};

/**************************************************************************************************************
 * End of struct and class definitions
**************************************************************************************************************/

/**************************************************************************************************************
 * Helper functions
**************************************************************************************************************/

bool isEscapeSequence(unsigned int index, string check) {
    return check[index] == '\r' && index+1 < check.length() && check[index+1] == '\n';
}

string readInput(string target) {

    ifstream input( target, ios_base::binary );
    string inputText ((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    return inputText;

}

void writeBits(string target, string bits) {

    ofstream output (target);
    for (unsigned int i = 0; i < bits.length()/8; i++) {
        output << char(bitset<8>(bits.substr(8*i,8)).to_ulong());
    }
    output.close();

}

/**************************************************************************************************************
 * End helper functions
**************************************************************************************************************/

/**************************************************************************************************************
 * Primary functions
**************************************************************************************************************/

Tree createHuffmanTree(map<string,Node*> nodes) {
    unsigned int size = nodes.size();

    for (unsigned int index = 0; index < size-1; index++) {
        
        Node *least = nullptr;
        Node *least2 = nullptr;
        for (auto i : nodes) {

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
        superNode->set(least->characters+least2->characters, least->freq+least2->freq, least, least2);
        
        nodes.erase(least->characters);
        nodes.erase(least2->characters);
        nodes.insert(pair<string,Node*>(superNode->characters, superNode));

        if (index == size-2) {
            return Tree(superNode);
        }

    }

    return Tree();
}

void zip(string target) {
    
    string encode = readInput(target);

    string newEncode = "";
    map<char,int> frequencies;
    for (unsigned int i = 0; i < encode.length(); i++) {

        if (!isEscapeSequence(i, encode)) {
            newEncode += string(1, encode[i]);
            auto index = frequencies.find(encode[i]);
            if (index == frequencies.end()) {
                frequencies.insert(pair<char,int>(encode[i], 1));
            } else {
                index->second += 1;
            }
        }

    }
    encode = newEncode;

    if (frequencies.size() == 1) {

        string bits = bitset<8>(frequencies.begin()->first).to_string();
        bits += bitset<32>(frequencies.begin()->second).to_string();
        writeBits(target + ".zip", bits);
        return;
        
    }

    map<string,Node*> nodes;
    for (auto i : frequencies) {
        
        Node *copy = new Node;
        copy->set(string(1, i.first), i.second, nullptr, nullptr);
        nodes.insert(pair<string,Node*>(string(1, i.first), copy));

    }

    Tree huffman = createHuffmanTree(nodes);
    map<char,string> codes = huffman.generateCodes();

    string bits = bitset<32>(encode.length()).to_string();
    bits += bitset<8>(codes.size()).to_string();
    bits += huffman.encode();
    for (unsigned int i = 0; i < encode.length(); i++) {
        bits += codes.at(encode[i]);
    }

    if (bits.length()%8 != 0) {
        unsigned int originalBitLength = bits.length();
        for (unsigned int i = 0; i < 8-(originalBitLength%8); i++) {
            bits += "0";
        }
    }

    writeBits(target + ".zip", bits);
}

void extract(string target) {
    
    string decode = readInput(target);

    string allBits = "";
    for (unsigned int i = 0; i < decode.length(); i++) {
        if (!isEscapeSequence(i, decode)) {
            allBits += bitset<8>(decode[i]).to_string();
        }
    }

    if (allBits.length() == 40) {
        string newFilename;
        for (unsigned char i = 0; i < target.length(); i++) {
            if (target.substr(i) == ".zip") {
                newFilename = target.substr(0,i);
            }
        }
        ofstream newFile (newFilename);

        for (unsigned int i = 0; i < bitset<32>(allBits.substr(8)).to_ulong(); i++) {
            newFile << decode[0];
        }
        return;
    }

    unsigned int numChars = bitset<32>(allBits.substr(0,32)).to_ulong();
    unsigned int charsInTree = bitset<8>(allBits.substr(32,8)).to_ulong();

    allBits = allBits.substr(40);

    SubtreeStack subtreeStack;
    unsigned int charsFound = 0;
    unsigned int bitIndex = 0;
    while (charsFound != charsInTree || subtreeStack.getSize() != 1) {

        if (allBits[bitIndex] == '1') {
            Node *leaf = new Node;
            leaf->set(string(1, bitset<8>(allBits.substr(bitIndex+1,8)).to_ulong()), nullptr, nullptr);
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

    string newFilename;
    for (unsigned char i = 0; i < target.length(); i++) {
        if (target.substr(i) == ".zip") {
            newFilename = target.substr(0,i);
        }
    }
    ofstream newFile (newFilename);

    unsigned int charsExtracted = 0;
    Node *selectedNode = huffman.head;
    for (unsigned int i = 0; i < allBits.length(); i++) {
        
        if (allBits[i] == '0') {
            selectedNode = selectedNode->left; 
        } else {
            selectedNode = selectedNode->right;
        }

        if (selectedNode->left == nullptr) {
            newFile << selectedNode->characters;
            selectedNode = huffman.head;
            charsExtracted += 1;
        }

        if (charsExtracted == numChars) {
            break;
        }
    }

    newFile.close();
}

/**************************************************************************************************************
 * End primary functions
**************************************************************************************************************/

/**************************************************************************************************************
 * Main function
**************************************************************************************************************/

int main(int argc, char *argv[]) {

    string option = argv[1];
    string file = argv[2];

    if (option == "-z") {
        zip(file);
    } else if (option == "-e") {
        extract(file);
    }

}

/**************************************************************************************************************
 * End main function
**************************************************************************************************************/