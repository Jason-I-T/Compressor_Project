/*
 * Creates a new process to compress a file using MyCompress. Source & destination file 
 * names presented as command-line arguments... Main process waits...
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void childProcess() {
    printf("starting compression...\n");
}

void parentProcess() {
    printf("finished compression: wrote to DESTINATION using SOURCE\n");
}

int main() {
    pid_t pid;
    pid = fork(); // Creating new process

    if(pid < 0) {
        printf("ERROR\n"); 
    }

    else if(pid == 0)
        childProcess();
    else {
        wait(&pid);
        printf("waiting...\n");
        printf("compressed...\n");
        parentProcess(); 
    }
}
