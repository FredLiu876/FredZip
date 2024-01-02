#include <bitset>
#include <fstream>
#include <string>
#include "ZipIO.h"

// check for window's escape sequence
bool isWindowsEscapeSequence(unsigned int index, string check) {
    return check[index] == '\r' && index+1 < check.length() && check[index+1] == '\n';
}

// remove all window's escape sequence, replacing \r\n with \n only
string removeWindowsEscapeSequence(string encode) {
    string newEncode = "";
    for (unsigned int i = 0; i < encode.length(); i++) {
        if (!isWindowsEscapeSequence(i, encode)) {
            newEncode += string(1, encode[i]);
        }
    }
    return newEncode;
}

// return file text as one string
string readInput(string target) {
    ifstream input( target, ios_base::binary );
    string inputText ((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    inputText = removeWindowsEscapeSequence(inputText);
    return inputText;
}

// read first bit of a string
char readFirstBit(string s) {
    return (bitset<8>(s[0]).to_string())[0];
}

// write stream of bits into textfile
void writeBits(string writeTo, string bits) {
    // pad to multiple of 8
    if (bits.length() % 8 != 0) {
        unsigned int originalBitLength = bits.length();
        int numPaddingNeeded = 8 - (originalBitLength % 8);
        for (unsigned int i = 0; i < numPaddingNeeded; i++) {
            bits += "0";
        }
    }
    ofstream output (writeTo);
    for (unsigned int i = 0; i < bits.length()/8; i++) {
        output << char(bitset<8>(bits.substr(8*i,8)).to_ulong());
    }
    output.close();
}

void writeFile(string writeTo, string text) {
    ofstream newFile (writeTo);
    newFile << text;
    newFile.close();
}