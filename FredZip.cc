#include <iostream>
#include <string>
#include "ZipIO.h"
#include "HuffmanTree.h"
#include "LZ77.h"

using namespace std;

const char HUFFMAN_COMPRESSION = '0';
const char HYBRID_COMPRESSION = '1';

void zip(string target, string writeTo) {
    string encode = readInput(target);

    string encoding1 = compressionHuffman(encode);
    encoding1.insert(0, string(1, HUFFMAN_COMPRESSION));

    string encoding2 = compressionLZ77(encode);
    encoding2 = compressionHuffman(encoding2);
    encoding2.insert(0, string(1, HYBRID_COMPRESSION));

    string filename = target;
    if (writeTo != "") {
        filename = writeTo;
    }
    if (encoding1.length() <= encoding2.length()) {
        writeBits(filename + ".fredzip", encoding1);
    } else {
        writeBits(filename + ".fredzip", encoding2);
    }
}

void extract(string target, string writeTo) {
    string decode = readInput(target);

    char compressionMethod = readFirstBit(decode);
    string result = decompressionHuffman(decode);
    if (compressionMethod == HYBRID_COMPRESSION) {
        result = decompressionLZ77(result);
    }

    string filename = writeTo;
    if (filename == "") {
        for (int i = 0; i < target.length(); i++) {
            if (target.substr(i) == ".fredzip") {
                filename = target.substr(0,i);
                break;
            }
        }
    }
    writeFile(filename, result);
}

/**************************************************************************************************************
 * Main function
**************************************************************************************************************/

int main(int argc, char *argv[]) {
    if (argc == 1 || argc >= 5) {
        cout << "Incorrect number of parameters, please use FredZip help or FredZip -h to see commands." << endl;
        return 1;
    }

    string option = argv[1];
    string writeFile = "";
    string targetFile = "";

    if (argc >= 3) {
        targetFile = argv[2];
    }
    if (argc == 4) {
        writeFile = argv[3];
    }

    if (option == "-z") {
        zip(targetFile, writeFile);
    } else if (option == "-e") {
        if (targetFile.length() < 8 || targetFile.substr(targetFile.length() - 8) != ".fredzip") {
            cout << "Targeted file is not a fredzip file" << endl;
            return 1;
        } else {
            extract(targetFile, writeFile);
        }
    } else if (option == "-h" || option == "help") {
        cout << "-z filename (newFilename)? --> Zip a file with optional parameter of custom zip file name" << endl;
        cout << "-e zipFilename (newFilename)? --> Extract a zip file with optional parameter of custom new file name" << endl;
    } else {
        cout << "Unknown parameters, please use FredZip help or FredZip -h to see commands." << endl;
        return 1;
    }
}