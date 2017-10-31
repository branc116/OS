#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define MAX_K 5
#define MAX_M 5

struct dekker_flags {
    int wants_to_enter[2];
    int turn;
};
typedef struct dekker_flags dekker_flags_t;
dekker_flags_t* flags;
void my_lock(int i) {
    flags->wants_to_enter[i] = 1;
    while (flags->wants_to_enter[(i + 1)%2]) {
        if (flags->turn != i) {
            flags->wants_to_enter[i] = 0;
            while(flags->turn != i) {
                sleep(1);
            }
        }
    }
}
void my_un_lock(int i) {
    flags->turn = (i + 1)%2;
    flags->wants_to_enter[i] = 0;
}

void ispis(int i, int k, int m) {
    printf("Proces: %d, k: %d, m: %d\n", i, k, m);
    return;
}

void proc(int i) {
    int k = 0;
    int m = 0;
    for (;k<MAX_K;k++) {
        my_lock(i);
        for (m = 0;m<MAX_M;m++) {
            ispis(i, k, m);
        }
        my_un_lock(i);
        sleep(1);
    }
    return;
}

int main(void) {
    int pid;
    flags = mmap(NULL, sizeof(dekker_flags_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    pid = fork();
    if (pid != 0) {
        sleep(1);
        proc(0);
        printf("Exiting process 0\n");
        return 0;
    }
    pid = fork();
    if (pid != 0) {
        proc(1);
        printf("Exiting process 1\n");
        return 0;
    }
    printf("Exiting main\n");
    return 0;
}