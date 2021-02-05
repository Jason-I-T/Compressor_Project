#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

void processString(string bits, fstream& outFile) {
    char data[bits.size() + 1];
    strcpy(data, bits.c_str());

    int counter = 1;
    for(int i = 0; i < bits.size(); i++) {
        if((*(data + i) - *(data + i + 1)) == 0)
               counter++; 
        else {
            if(counter < 16) {
                while(counter > 0) {
                    outFile << data[i];
                    counter--;
                }
            }

            else {
                if(data[i] == '1')
                    outFile << "+" << counter << "+"; 
                else
                    outFile << "-" << counter << "-";
            }       

            counter = 1; 
        }
    }
}

int main() {
    fstream inFile;
    fstream outFile;
    string str;
    string token;

    inFile.open("public/compress_me.txt", ios::in);
    outFile.open("public/i_compressed.txt", ios::out);

    // Loop reads file, sends strings to get compressed and written to destination file.
    while(getline(inFile, str)) {
        istringstream iss(str);
        if(str.find(' ') != string::npos) {
            while(getline(iss, token, ' ')) {
                processString(token, outFile);
                outFile << " ";
            }
            outFile << "\n";
            continue;
        }
        processString(str, outFile);
        outFile << "\n";
    }
    inFile.close();
    outFile.close();
}
