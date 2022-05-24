#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <bitset>
#include "IndexList.h"
#include "node.h"
#include "HuffmanTree.h"
#include "SubtreeStack.h"

using namespace std;

/**************************************************************************************************************
 * Helper functions
**************************************************************************************************************/

bool isEscapeSequence(unsigned int index, string check) {
    return check[index] == '\r' && index+1 < check.length() && check[index+1] == '\n';
}

bool isNumber(char check) {
    return check >= 48 && check <= 57;
}

// return file text as one string
string readInput(string target) {

    ifstream input( target, ios_base::binary );
    string inputText ((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    return inputText;

}

// write stream of bits into textfile
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

string compressionLZ77(string encode) {

    map<char,IndexList*> indexes;
    for (unsigned int i = 0; i < encode.length(); i++) {

        auto iter = indexes.find(encode[i]);

        // create new linked list containing indexes if this is first time character has appeared
        if (iter == indexes.end()) {
            
            IndexList *newIndexList = new IndexList;
            newIndexList->push(i);
            indexes.insert(pair<char,IndexList*>(encode[i], newIndexList));
        } else {
            
            //check for matches for all previous indexes of character
            IndexList::IndexNode *selected = iter->second->head;
            unsigned int constantSize = iter->second->getSize();
            pair<unsigned int,unsigned int> greatestLength (0, 0);
            for (unsigned int k = 0; k < constantSize; k++) {
                if (k != 0) {
                    selected = selected->next;
                }
                // check match
                unsigned int distance = i-selected->index;
                pair<unsigned int,unsigned int> distanceLength (0, distance);
                for (unsigned int j = 0; j < encode.length()-i; j++) {
                    if (j >= distance) {
                        if (encode[i+j] != encode[i+(j%distance)]) {
                            distanceLength.first = j;
                            break;
                        }
                    } else if (encode[selected->index+j] == encode[i+j]) {
                        
                    } else {
                        distanceLength.first = j;
                        break;
                    }
                    if (j >= encode.length()-i-1) {
                        distanceLength.first = j+1;
                    }
                }

                // use longest match
                if (distanceLength.first > greatestLength.first) {
                    greatestLength = distanceLength;
                }
            }
            // replace match with distance length pair if longest match is long enough
            string replacement = "~" + to_string(greatestLength.second) + "$" + to_string(greatestLength.first) + "~";
            if (greatestLength.first > replacement.length()) {
                encode = encode.substr(0,i) + replacement + encode.substr(i+greatestLength.first);
                i += replacement.length();

            // push new index for character if no matches are long enough
            } else {
                indexes.at(encode[i])->push(i);
            }
        }
    }
    for (auto it = indexes.begin(); it != indexes.end(); it++) {
        delete it->second;
    }
    return encode;
}

void extractLZ77(string priorLZ77, string target) {

    string newFilename;
    for (unsigned char i = 0; i < target.length(); i++) {
        if (target.substr(i) == ".zip") {
            newFilename = target.substr(0,i);
        }
    }
    ofstream newFile (newFilename);

    for (unsigned int i = 0; i < priorLZ77.length(); i++) {
        if (priorLZ77[i] == '~') {

            //check if it is distance length pair or a normal character
            unsigned int distance = 0;
            unsigned int length = 0;
            bool distanceFound = false;
            bool lengthFound = false;
            bool dollarFound = false;
            int dollarIndex = 0;

            for (unsigned int j = 1; j < priorLZ77.length()-i; j++) {

                if (priorLZ77[i+j] == '$' && !dollarFound) {
                    if (distanceFound) {
                        distance = stoi(priorLZ77.substr(i+1, j-1));
                        dollarFound = true;
                        dollarIndex = j;
                    } else {
                        break;
                    }
                } else if (priorLZ77[i+j] == '~') {
                    if (dollarFound && lengthFound) {
                        length = stoi(priorLZ77.substr(i+dollarIndex+1, j-dollarIndex-1));
                    } else {
                        break;
                    }
                } else if (isNumber(priorLZ77[i+j])) {
                    if (!dollarFound) {
                        distanceFound = true;
                    } else {
                        lengthFound = true;
                    }
                } else {
                    break;
                }

            }

            // regular character
            if (distance == 0 || length == 0) {
                newFile << priorLZ77[i];

            // valid distance length pair, write to new file
            } else {
                for (unsigned int j = 0; j < length; j++) {
                    newFile << priorLZ77[i-distance+(j%distance)];
                }
                string distanceLengthString = "~" + to_string(distance) + "$" + to_string(length) + "~";
                i += distanceLengthString.length()-1;
            }

        } else {
            newFile << priorLZ77[i];
        }
    }

    newFile.close();
}

// create huffman tree based on character frequencies
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

void zip(string target, string writeFile) {
    
    string encode = readInput(target);
    // remove \r from \r\n escape sequence
    string newEncode = "";
    map<char,int> frequencies;
    for (unsigned int i = 0; i < encode.length(); i++) {

        if (!isEscapeSequence(i, encode)) {
            newEncode += string(1, encode[i]);
        }

    }
    encode = newEncode;
    encode = compressionLZ77(encode);
    for (unsigned int i = 0; i < encode.length(); i++) {
        auto index = frequencies.find(encode[i]);
        if (index == frequencies.end()) {
            frequencies.insert(pair<char,int>(encode[i], 1));
        } else {
            index->second += 1;
        }
    }

    if (frequencies.size() == 1) {

        string bits = bitset<8>(frequencies.begin()->first).to_string();
        bits += bitset<32>(frequencies.begin()->second).to_string();

        if (writeFile != "") {
            writeBits(writeFile + ".zip", bits);
        } else {
            writeBits(target + ".zip", bits);
        }

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
    if (writeFile != "") {
        writeBits(writeFile + ".zip", bits);
    } else {
        writeBits(target + ".zip", bits);
    }
}

void extract(string target, string writeFile) {
    
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

    string priorLZ77 = "";
    unsigned int charsExtracted = 0;
    Node *selectedNode = huffman.head;
    for (unsigned int i = 0; i < allBits.length(); i++) {
        
        if (allBits[i] == '0') {
            selectedNode = selectedNode->left; 
        } else {
            selectedNode = selectedNode->right;
        }

        if (selectedNode->left == nullptr) {
            priorLZ77 += selectedNode->characters;
            selectedNode = huffman.head;
            charsExtracted += 1;
        }

        if (charsExtracted == numChars) {
            break;
        }
    }

    if (writeFile != "") {
        extractLZ77(priorLZ77, writeFile + ".zip");
    } else {
        extractLZ77(priorLZ77, target);
    }
}

/**************************************************************************************************************
 * End primary functions
**************************************************************************************************************/

/**************************************************************************************************************
 * Main function
**************************************************************************************************************/

int main(int argc, char *argv[]) {

    string option = argv[1];
    string writeFile = "";
    string targetFile = "";

    if (argc >= 3) {
        targetFile = argv[2];
    }

    if (argc >= 4) {
        writeFile = argv[3];
    }

    if (option == "-z") {
        zip(targetFile, writeFile);
    } else if (option == "-e") {
        if (targetFile.substr(targetFile.length()-4) == ".zip") {
            extract(targetFile, writeFile);
        } else {
            cout << "Targeted file is not zip file" << endl;
            return 1;
        }
    } else if (option == "-h" || option == "help") {
        cout << "-z filename (newFilename)? --> Zip a file with optional parameter of custom zip file name" << endl;
        cout << "-e zipFilename (newFilename)? --> Extract a zip file with optional parameter of custom new file name" << endl;
    } else {
        cout << "Unknown parameters, please use FredZip help or FredZip -h to see commands." << endl;
        return 1;
    }

}

/**************************************************************************************************************
 * End main function
**************************************************************************************************************/