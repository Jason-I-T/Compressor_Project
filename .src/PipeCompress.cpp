#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    string line;

    fstream inFile;
    fstream outFile;
    string source = "public/compress_me.txt";
    string destination = "public/compressed_by_pipe.txt";

    char newline[1] = {'\n'}, space[1] = {' '};

    pid_t pid;
    int pfd[2];
    
    if(pipe(pfd) == -1) {
        cout << "ERROR: Pipe failed" << endl;
        exit(1); 
    } 

    pid = fork();

    if(pid < 0) {
        cout << "ERROR: Fork failed" << endl; 
    } else if(pid == 0) { // Child
        close(pfd[1]);     
        char ch[1];
        while(read(pfd[0], ch, 1) > 0) {
            cout << ch[0]; 
        }
    } else { // Parent
        inFile.open(source, ios::in);
        close(pfd[0]);
        while(getline(inFile, line)) {
            istringstream iss(line);
            if(line.find(' ') != string::npos) {
                while(getline(iss, line, ' ')) {
                    write(pfd[1], line.c_str(), line.size() + 1); 
                    write(pfd[1], space, 1);
                }            
                write(pfd[1], newline, 1);
            } else {
                write(pfd[1], line.c_str(), line.size() + 1); 
                write(pfd[1], newline, 1);
            }
        }
        close(pfd[1]);
    }
}