#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <bitset>

using namespace std;

int main() {
    ofstream newFile ("output1.txt");
    string testString = "\rb";
    newFile << testString;
    newFile.close();

    ifstream inputFile( "output1.txt", ios_base::binary );
    string encode ((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    string newString = "";
    for (unsigned int i = 0; i < encode.length(); i++) {
        if (encode[i] == '\r') {
            newString += "\\r";
        } else if (encode[i] == '\n') {
            newString += "\\n";
        } else if (encode[i] == '\t') {
            newString += "\\t";
        } else {
            newString += string(1, encode[i]);
        }
    }
    cout << newString;
    cout << " Here" << endl;
    inputFile.close();

    ofstream outputFile ("output2.txt");
    string allBits = "";
    for (unsigned int i = 0; i < testString.length(); i++) {
        allBits += bitset<8>(testString[i]).to_string();
    }
    outputFile << allBits;
    outputFile << " Here" << endl;

    allBits = "";
    for (unsigned int i = 0; i < encode.length(); i++) {
        if (!(encode[i] == '\r' && i+1 < encode.length() && encode[i+1] == '\n')) {
            allBits += bitset<8>(encode[i]).to_string();
        }
    }
    outputFile << allBits;
    outputFile << " Here" << endl;
    outputFile.close();
}