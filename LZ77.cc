#include <string>
#include "LZ77.h"

string compressionLZ77(string encode) {
    string encodedString = "";

    for (unsigned int i = 0; i < encode.length(); i++) {
        int longestMatch = 0;
        int beginningOfLongestMatch = 0;

        unsigned int lookbackWindowStart = 0;
        if (i > DISTANCE_AND_LOOKBACK_BUFFER_LENGTH) {
            lookbackWindowStart = i - DISTANCE_AND_LOOKBACK_BUFFER_LENGTH;
        }
        
        for (unsigned int j = lookbackWindowStart; j < i; j++) {
            int k = 0;
            for (; j + k < i; k++) {
                if (encode[i + k] != encode[j + k]) {
                    break;
                }
            }
            int lengthOfCurrentMatch = k;
            if (lengthOfCurrentMatch > longestMatch) {
                longestMatch = lengthOfCurrentMatch;
                beginningOfLongestMatch = j;
            }
        }

        if (longestMatch == 0) {
            encodedString += "0";
            encodedString += encode[i];
        } else {
            encodedString += to_string(i - beginningOfLongestMatch) + "," + to_string(longestMatch) + ",";
            i += longestMatch - 1;
        }
    }

    return encodedString;
}

string decompressionLZ77(string encoded) {
    string decodedString = "";
    unsigned int i = 0;
    while (i < encoded.length()) {
        if (encoded[i] == '0') {
            // This must be (0, symbol) pair
            decodedString += encoded[i + 1];
            i += 2;
        } else {
            // This must be (distance, length) pair
            string distanceStr = "";
            string lengthStr = "";
            while (encoded[i] != ',') {
                distanceStr += encoded[i];
                i++;
            }
            i++;
            while (encoded[i] != ',') {
                lengthStr += encoded[i];
                i++;
            }
            i++;
            int distance = stoi(distanceStr);
            int length = stoi(lengthStr);
            int beginningOfMatch = decodedString.length() - distance;
            for (int j = 0; j < length; j++) {
                decodedString += decodedString[beginningOfMatch + j];
            }
            
        }
    }
    return decodedString;
}