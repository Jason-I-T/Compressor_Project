#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>

using namespace std;

// write a function to write to the destination file
// ..........
// ..........
// ..........
// ..........
int main() {
    fstream inFile;
    string str;
    string token;

    inFile.open("public/compress_me.txt", ios::in);

    // Loop reads file, sends strings to get compressed and written to destination file.
    while(getline(inFile, str)) {
        istringstream iss(str);
        if(str.find(' ') != string::npos) {
            while(getline(iss, token, ' ')) {
                cout << token << "\n";
            }
            continue;
        }
        cout << str << "\n";
    }
    inFile.close();
}
