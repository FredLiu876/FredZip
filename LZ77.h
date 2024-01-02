#ifndef LZ77_H
#define LZ77_H
#include <string>

using namespace std;

const int DISTANCE_AND_LOOKBACK_BUFFER_LENGTH = 999;

string compressionLZ77(string encode);
string decompressionLZ77(string decode);

#endif