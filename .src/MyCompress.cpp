#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <cstring>

using namespace std;

void processString(string bits) {
    char data[bits.size() + 1];
    strcpy(data, bits.c_str());
    ofstream outfile;
    outfile.open("public/destination.txt", ios_base::app);


    int counter = 1;
    cout << bits << "\n";
    for(int i = 0; i < bits.size(); i++) {
        if((*(data + i) - *(data + i + 1)) == 0)
               counter++; 
        else {
            if(counter < 16) {
                   // Write compressed bits into destination file
                   cout << counter << "\n"; 
                    for (int j = 0; j < counter; j++){
                       outfile << data[i];
                   }
            }

            else {
                   // Iterate over counter and write the appropriate bits in destination file until counter = 0.
                    if (data[i] == '1'){
                        outfile << "+" << counter << "+";
                   }
                   else {
                        outfile << "-" << counter << "-";
                   }
                   cout << counter << "\n";  
            }       

            counter = 1; 
        }
    }
    outfile << " ";
}

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
                processString(token);
            }
            continue;
        }
        processString(str);
    }
    inFile.close();
}