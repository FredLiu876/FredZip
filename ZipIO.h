#ifndef ZIPIO_H
#define ZIPIO_H
#include <string>

using namespace std;

string readInput(string target);
char readFirstBit(string s);
void writeBits(string writeTo, string bits);
void writeFile(string writeTo, string text);

#endif