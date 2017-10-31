#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define MAX 40
long long n;
void *increment(void *par) {
    int inc = *((int*)(par));
    while(inc--)
        n++;
    return;
}

int main(int argc, char *argv[]){
    pthread_t threads[MAX];
    void* retValue;
    int i = 0;
    int m = atoi(argv[1]);
    int inc = atoi(argv[2]);
    for(;i<m;i++) {
        pthread_create(&threads[i], NULL, increment, &inc);
    }
    printf("threads created \n");
    for(i = 0;i<m;i++) {
        pthread_join(&threads[i], &retValue);
    }
    printf("Value of global variable is: %d, n/milion: %d\n", n, n/1000000);
    return 0;
}