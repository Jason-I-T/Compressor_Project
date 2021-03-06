#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;

int chunks = 288;

void processString(string bits, vector<string>& compressed) {
    char data[bits.size() + 1];
    strcpy(data, bits.c_str());
    int counter = 1;
    string line;
    for(int i = 0; i < bits.size(); i++) {
        if((*(data + i) - *(data + i + 1)) == 0)
            counter++;
        else {
            if(counter < 16) {
                while(counter > 0) {
                    string s(1, data[i]);
                    line = to_string(data[i]);
                    compressed.push_back(s);
                    //cout << data[i];
                    counter--;
                }
            }
            else {
                if(data[i] == '1') {
                    line = "+" + to_string(counter) + "+";
                    compressed.push_back(line);
                    //cout << line;
                }
                else {
                    line = "-" + to_string(counter) + "-";
                    compressed.push_back(line);
                    //cout << line;
                }
            }
            counter = 1;
        }
    }
}

int numberOfLines(fstream& inFile) {
    string line;
    int lineCount = 0;

    while (getline(inFile, line)) {
        lineCount++;
    }
    inFile.close();
    return lineCount;
}

//Divides file into chunks and stores them into an array of vectors
void divideFile(fstream& inFile, vector<string> parts[], int numOfLines, int index) {
    string line;
    int lineCount = 0;
    
    while (getline(inFile, line)) {
        if (lineCount >= numOfLines / chunks) {
            index+=1;
            lineCount = 0;
        }
        parts[index].push_back(line);
        lineCount++;
    }
    inFile.close();
}

void iterateThroughVectors(vector<string>& compressed, vector<string>& parts) {
    string str;
    string token;
    for (int i = 0; i < parts.size(); i++){
        str = parts.at(i);
        istringstream iss(str);
        if (str.find(' ') != string::npos) {
            while (getline(iss, token, ' ')) {
                processString(token, compressed);
                //cout << " ";
                compressed.push_back(" ");
            }
            compressed.push_back("\n");
            //cout << "\n";
            continue;
        }
        processString(str, compressed);
        //cout << "\n";
    }

    cout << "-----------------Size: " << compressed.size() << "------------------\n";
}

void writeToParent(int fd[], vector<string>& compressed) {
    close(fd[0]);
    int sizeOfCompressed = compressed.size();
    cout << "Vector size: " << compressed.size() << "\n";
    write(fd[1], &sizeOfCompressed, sizeof(int));
    int n = 0;
    
    while (n < sizeOfCompressed) { 
        string temp = compressed.at(n);
        // cout << "N: " << n << " " << compressed.at(n) << "\n";
        // cout << compressed.at(n+1) << "\n";
        // cout << sizeOfCompressed << "\n";
        int stringLen = temp.size() + 1;
        write(fd[1], &stringLen, sizeof(int));
        write(fd[1], temp.c_str(), stringLen);
        ++n;
    }

    close(fd[1]);
    cout << "Wrote to parent\n";
}

void writeToOutputVector(int fd[], vector<string>& compressed) {
    close(fd[1]);
    int numberOfStrings;
    int m = 0;
    read(fd[0], &numberOfStrings, sizeof(int));
    cout << "\nNumber of strings: " << numberOfStrings << "\n";
    cout << "Reading vector\n";

    while (m < numberOfStrings) {
        int sizeOfString;
        read(fd[0], &sizeOfString, sizeof(int));
        char s [sizeOfString];
        read(fd[0], &s, sizeof(char) * sizeOfString);
        compressed.push_back(s);
        ++m;   
    }
    close(fd[0]);
    cout << "Finished reading vector\n\n";
}

void writeToFile(fstream& outFile, vector<string> compressed[]) { 
    for (int i = 0; i < chunks; i++) { 
        for (int j = 0; j < compressed[i].size(); j++) { 
            outFile << compressed[i].at(j);
        }
    }
    outFile.close();
}

int main() {
    clock_t tStart = clock();
    fstream inFile;
    fstream outFile;
    fstream newFile;
    inFile.open("test2.txt", ios::in);
    newFile.open("test2.txt", ios::in);
    outFile.open("destination.txt", ios::out);
    vector<string> parts[chunks];
    vector<string> compressed[chunks];
    pid_t pids[chunks];
    int fd[chunks][2];
    
    for (int i = 0; i < chunks; i++) {
        if (pipe(fd[i]) == -1) {
            cout << "Error creating pipes\n";
        }
    }

    for (int i = 0; i < chunks; i++) {
        divideFile(newFile, parts, numberOfLines(inFile), i);
    }

    for (int i = 0; i < chunks; i++) {
        if ((pids[i] = fork()) < 0) {

        }
        else if (pids[i] == 0) {
            cout << "Reading from file\n";
            iterateThroughVectors(compressed[i], parts[i]);
            writeToParent(fd[i], compressed[i]);
            exit(0); 
        }
    }
    // int status;
    // int n = chunks;
    // pid_t pid;
    // while (n > 0) {
    //     pid = wait(&status);
    //     --n;
    // }

    for (int j = 0; j < chunks; j++) {
        wait(NULL);
    }
    
    cout << "Here\n";


    cout << "-----------------------Writing to parent-----------------------\n";
    for (int i = 0; i < chunks; i++) {
        writeToOutputVector(fd[i], compressed[i]);
    }  

    writeToFile(outFile, compressed);

    cout << "-----------------------Time taken: " << (double) (clock() - tStart) / CLOCKS_PER_SEC << "-----------------------\n";
    return 0;
}