#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

// Biggest issue with this implementation - if there are multiple compressed bits of the same type in one string,
// then we cannot handle it based on the "tokenizing" system. However, if the compressed bits are across 
// different strings or are different types in one string then we are good. Has to do with the upper/lower bound
int getCount(char arr[], int n, char c) {
    int i = 0, j;
    for(i = 0; i < n; i++)
        if(arr[i] == c)
            break;
    if(i >= n - 1)
        return 0;
    for(j = n - 1; j >= i + 1; j--) 
        if(arr[j] == c)
            break;
    if(j==i)
        return 0;
    return(j - i - 1);
}

// Decompress the bits, write into destination file
void decompress(string str, char c) {
    int n_compressedBits = stoi(str);
    char bit[2];
    
    if(c == '-')
        bit[0] = '0';
    else
        bit[0] = '1';
    for(int i = 0; i < n_compressedBits; i++)
        cout << bit[0];
    //cout << n_compressedBits << c;
    //Cast string into int, determine whether we will write 1s or 0s, loop until we are done writing.
    return;
}

void processString(string bits) {
    char data[bits.size() + 1];
    strcpy(data, bits.c_str());

    for(int i = 0; i < bits.size(); i++) {
        if(data[i] == '0' || data[i] == '1') {
            cout << data[i];
        }
        else {
            int count = getCount(data, sizeof(data), data[i]); // number of digits between '+' or '-'
            decompress(string(&bits[i+1], &bits[i+count + 1]), data[i]); //string of values between '+' or '-'
            i += count + 1; // Increment i by how much we skipped
        }
    }
    cout << "\n"; 
}

int main() {
    fstream inFile;
    string str;
    string token;

    inFile.open("public/i_compressed.txt", ios::in);

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
