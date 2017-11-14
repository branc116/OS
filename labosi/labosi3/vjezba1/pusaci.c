#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#define N_SMOKERS 100
struct therad_sem {
    pthread_t thread;
    sem_t sem;
    int id;
    int n;
};
typedef struct therad_sem thread_sem_t;
thread_sem_t somker_sem[N_SMOKERS];
thread_sem_t shop_keeper_sem;

char shop_keeper_message[][20] = {"duhan i papir", "duhan i rizle", "rizle i papir"};
char smoker_has[][20] = {"rizle", "papir", "duhan"};
/*
0 - duhan & papir
1 - duhan & filteri
2 - filteri & papir
*/
void *shop_keeper(void* args) {
    int rand_num;
    while(1) {
        sem_wait(&(shop_keeper_sem.sem));
        rand_num = rand() % 3;
        printf("Trgovac : Stavlja na stol %s.\n", shop_keeper_message[rand_num]);
        sem_post(&(somker_sem[rand_num].sem));
    }
}
 
void *pusac(void* p) {
    int smoke_time;
    thread_sem_t cur_thread = *((thread_sem_t *)p);
    printf("Pusac #%d: Ima %s, treba %s.\n", cur_thread.n + 1, smoker_has[cur_thread.id], shop_keeper_message[cur_thread.id]);
    while(1) {
        sem_wait(&(somker_sem[cur_thread.id].sem));
        printf("Pusac #%d: Uzima %s.\n", cur_thread.n + 1, shop_keeper_message[cur_thread.id]);
        sem_post(&(shop_keeper_sem.sem));
        smoke_time = rand() % 20 + 10;
        printf("Pusac #%d: Mota i pusi slijedecih %ds.\n\n", cur_thread.n + 1, smoke_time);
        sleep(smoke_time);
    }
}

int main(void) {
    int i = 0;
    srand(time(NULL));
    sem_init(&(shop_keeper_sem.sem), 0, 1);
    for(;i<3;i++) {
        sem_init(&(somker_sem[i].sem), 0, 0);
    }
    pthread_create(&(shop_keeper_sem.thread), NULL, shop_keeper, 0);
    for(i = 0;i<N_SMOKERS;i++) {
        if (!(i % 4))
            sleep(1);
        somker_sem[i].id = rand() % 3;
        somker_sem[i].n = i;
        pthread_create(&(somker_sem[i].thread), NULL, pusac, &somker_sem[i]);
    }
    sleep(1000);
    return 0;
}