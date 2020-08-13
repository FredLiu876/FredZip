#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <bitset>

using namespace std;

int main(int argc, char *argv[]) {

    for (unsigned int i = 0; i < argc; i++) {
        cout << i << " -> " << argv[i] << endl;
    }

}