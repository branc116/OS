#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int sig[]={SIGUSR1, SIGUSR2, SIGQUIT, SIGTERM, SIGINT};
int pid;
void kill_main(int sig){
    printf("exiting");
    exit(0);
    return;
}

int main(int argc, char *argv[]){
    int cur_sig, cur_sleep;
    pid = atoi(argv[1]);
    sigset(SIGINT, kill_main);
    srand(time(NULL));
    while(1){
        cur_sig = rand() % 5;
        cur_sleep = rand() % 2 + 4;
        printf("Sending %d to %d\n", sig[cur_sig], pid);
        kill(pid, sig[cur_sig]);
        printf("Sleeping for %d sec\n", cur_sleep);
        sleep(cur_sleep);
    }
    return 0;
}