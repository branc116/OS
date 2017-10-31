#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define MAX_T 20
#define MAX_TABLE 20
struct lamport_flags {
    int trazim;
    int broj;
    int i;
    pthread_t thread;
};
typedef struct lamport_flags lamport_flags_t;

lamport_flags_t threads[MAX_T];
int tables[MAX_TABLE];
int n_tables;
int n_threads;
int next_num() {
    int i = 0, max = 0;
    for (;i<n_threads;i++) {
        max = max < threads[i].broj ? threads[i].broj : max;
    }
    return max + 1;
}

void my_lock(int i) {
    int j = 0;
    threads[i].trazim = 1;
    threads[i].broj = next_num();
    threads[i].trazim = 0;
    for (;j<n_threads;j++) {
        while(threads[j].trazim);
        while(threads[j].broj != 0 && (threads[j].broj < threads[i].broj || (threads[j].broj == threads[i].broj && j < i) ));
    }
}
void my_un_lock(int i) {
    threads[i].broj = 0;
}
int anything_free() {
    int j = 0;
    for (;j<n_tables;j++) {
        if (!tables[j])
            return 1;
    }
    return 0;
}
void print_tables() {
    int i = 0;
    for (;i<n_tables;i++) {
        if (tables[i])
            printf("%d ", tables[i]);
        else 
            printf("- ");
    }
    printf("\n");
}
void *oqpy(void *a) {
    int i = ((lamport_flags_t *)a)->i, j = 0;
    int randNum = 0;
    sleep(1);
    printf("Started thread %d\n", i);
    sleep(1);
    while(anything_free()) {
        j = 0;
        randNum = rand() % n_tables;
        while (++j < randNum || tables[j % n_tables] );
        printf("\nThread %d: odabirem stol %d\n", i + 1, j%n_tables);
        my_lock(i);
        if (tables[j%n_tables]){
            printf("Thread %d: neuspijelo rezervacija stola %d, stanje:\n", i + 1, j % n_tables);
        } else {
            tables[j%n_tables] = i + 1;
            printf("Thread %d: uspijesno rezerviranje stola %d, stanje:\n", i + 1, j % n_tables);
        }
        print_tables();
        my_un_lock(i);
        sleep(1);
    }
}

int main(int argc, char *argv[]){
    int i = 0;
    n_threads = atoi(argv[1]);
    n_tables = atoi(argv[2]);
    srand(time(NULL));
    for (;i<n_threads;i++) {
        threads[i].i = i;
        pthread_create(&(threads[i].thread), NULL, oqpy, &threads[i]);
    }
    printf("Threads created\n");
    for (i = 0;i<n_threads;i++) {
        pthread_join(threads[i].thread, NULL);
    }
    printf("Threads joined\n");
    return 0;
}