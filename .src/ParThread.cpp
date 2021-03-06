#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

const int chunks = 10;
int passes = 0;
vector<string> parts[chunks];

void processString(string bits, fstream &outFile) {
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
                    counter --; 
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

void *start(void * params) {
    string line;
    fstream* outFile = static_cast<fstream*>(params);
    for(int i = 0; i < parts[passes].size(); i++) {
        line = parts[passes].at(i);
        istringstream iss(line);
        if(line.find(' ') != string::npos) {
            //CRIT
            while(getline(iss, line, ' ')) {
                processString(line, *outFile);
                *outFile << " ";
            } 
            *outFile << "\n";
            //END CRIT
            continue;
        }
        //CRIT
        processString(line, *outFile);
        *outFile << endl;
        //END CRIT
    }
    
    cout << passes << endl;
    if(passes == chunks)
        return params;
    //CRIT
    passes++;
    //END CRIT
    return params;
}

int main(void) {
    pthread_t thread_id[chunks];
    int n = 0;

    // Getting number of lines and defining ranges....
    fstream lineCounter("public/big_compress_me.txt", ios::in);
    string line;
    while(getline(lineCounter, line))
        n++;
    lineCounter.close();

    int lineCount = 0;
    int index = 0;
    fstream reader("public/big_compress_me.txt", ios::in);
    for(int i = 0; i < chunks; i++) {
        while(getline(reader, line)) { // we lose some data here with the if statement...
            if(lineCount >= n / chunks) {
                index += 1; // Critical in populating parts...
                lineCount = 0;
            } 

            parts[index].push_back(line);
            lineCount++;
        }
    } // End

    // Starting compression steps...
    fstream outFile("public/big_destination.txt", ios::out);
    for(int i = 0; i < chunks; i++) { // Dispatch work
        pthread_create(&thread_id[i], NULL, &start, static_cast<void*>(&outFile));
    }
    for(int i = 0; i < chunks; i++) // End work
        pthread_join(thread_id[i], NULL);
}
