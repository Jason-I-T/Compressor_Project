#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    char command[50];
    printf("<MoreShell>\n");
    fgets(command, 50, stdin);
    
    //Check if first 4 characters are exit
    while (strncmp(command, "exit", 4) != 0) {
        //Create string array for the arguments
        char *args[4];

        //Set new line character to null
        command[strcspn(command, "\n")] = '\0';
        
        //Split the input by spaces
        char *tokens = strtok(command, " ");
        int count = 0;

        while (tokens != NULL){
            args[count] = (char*) malloc(5 * sizeof(char));
            strcpy(args[count], tokens);
            count++;
            tokens = strtok(NULL, " ");
        }
        
        //Set certain index in the args array to null
        args[count] = NULL;
        pid_t pid;

        //Create new Process;
        pid = fork(); 

        if (pid < 0) {
            printf("ERROR\n");
        }

        else if (pid == 0) {
            //Pass command and arguements to execvp
            printf("Running program...\n");
            execvp(args[0], args);
        }

        else {
            wait(&pid);
            printf("Finished\n");
        }
        
        //Set command string to null
        command[0] = '\0';

        //Take user input again
        printf("<MoreShell>\n");
        fgets(command, 50, stdin);
    }
}