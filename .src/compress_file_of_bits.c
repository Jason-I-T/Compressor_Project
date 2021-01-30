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
            printf("There is a whitespace...");
            tokens = strtok(str, " ");
            while(tokens != NULL) {
                // Process the string of bits here
                printf("\n%s", tokens);

                tokens = strtok(NULL, " ");
            }
            continue;
        }
        // Process the string of bits
        printf("%s", str);
    }

// Close the streams
    fclose(filePointer);
//  close(fileDescriptor);
}
