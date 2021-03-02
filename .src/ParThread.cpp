#include <iostream>
#include <pthread.h>
#include <fstream>

using namespace std;

int N = 10; // How we split up the file

void *compress(void *) {
    // Thread instructions
    pthread_exit(NULL);
}

int main(void) {
    pthread_t thread_id[10];
    fstream inFile;
    string source = "public/big_compress_me.txt";
    inFile.open(source, ios::in);
    string line;
    int n = 0;

    while(getline(inFile, line)) // Getting length of file (by lines)
        n++;

    for(int i = 0; i < N; i++) {
        int bounds[2];
        if(i == 0) {
            bounds[0] = 0; 
            bounds[1] = n/N; 
        } else if(i == N - 1) {
            bounds[0] = (n/N) * i + 1; 
            bounds[1] = (n/N) * (i + 1) + (n % N); 
        } else {
            bounds[0] = (n/N) * i + 1; 
            bounds[1] = (n/N) * (i + 1); 
        }

        cout << bounds[0] << " " << bounds[1] << endl;
        pthread_create(&thread_id[i], NULL, &compress, NULL);     
    }
    
    for(int i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);
}

/* THE CHOP
 * We divide the file by 10 (we want 10 threads). We do num_of_lines/10, and then get the remaining lines
 * from this operation by doing size % 10 and making the last thread take care of the last r lines.
 *
 * If we had 2054326 lines, each thread would do 10% (205432) lines, and the last thread would do the 10% plus
 *  6 (2054326 % 10) more lines.
 *
 * How are we gonna know where to start or end? The important parts are the beginning and end of a file, so take
 * care of these special cases? Everything in between (threads 2 - 9) will be starting where the last thread
 * left off + 1 and end n/10 lines later. 
 *
 * for first thread
 * start at 0 end at (n/10)
 *
 * for threads 2 - 9
 * start at (n/10)*(i) + 1, end at (n/10) * (i+1).
 *
 * for last thread (10)
 * start at (n/10)*(i) + 1, end at (n/10) * (i + 1) + (n % 10)
 */
