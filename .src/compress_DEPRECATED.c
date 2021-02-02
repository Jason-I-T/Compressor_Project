/* 
 * A C program that makes a compressed copy of an existing file of bits using system
 * calls for file manipulation. 
 * We have to adhere by 3 rules:
 *      Compress only the sequence of length >= 16 1's or 0's
 *      Sequence of 1's and 0's of length < 16 are copied as such
 *      The input file will have multiple segments of 1's and 0's separated by a 
 *      space or return.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//How do we return a string in C?
void process_string(char * str) {
    int count = 1;

    for(int i = 1; i < strlen(str); i++) {
        if (str[i] == str[i-1]) 
            count++;
        else {
            if (count >= 16) {
                // Compress bits... 
                // Can we make a string, and then make the buffer that newly created string?
                printf("%d\n", count);
            }  
            // Dump buffer into result string, reset buffer
            // How do we add to a string in C?
            // How can we reset our array that holds the bits we are reading?
            count = 1;
        }
    }
}

int main() {
    FILE *filePointer;
// Make this part better... how do we make a dynamically sized string?
    char str[1000]; 
    char * tokens;
// For the output file
    int fileDescriptor;

// Opening the file we want to compress...
    filePointer = fopen("public/compress_me.txt", "r");

// Creating/opening the destination file
// Last two parameters let us read/write the file. Gives perms to everyone (hopefully)
//  fileDescriptor = creat("public/i_compressed.txt", S_IRWXO | S_IRWXU);

// Implementation of the file reader...
    while(fgets(str, 1000, filePointer) != NULL) {
        if(strchr(str, ' ') != NULL) {
            printf("There is a whitespace...\n");
            tokens = strtok(str, " ");
            while(tokens != NULL) {
                // Process the string of bits here
                //printf("\n%s", process_string(tokens));
                process_string(tokens);

                tokens = strtok(NULL, " ");
            }
            continue;
        }
        // Process the string of bits
        //printf("%s", str);
        process_string(str);
    }

// Close the streams
    fclose(filePointer);
//  close(fileDescriptor);
}
