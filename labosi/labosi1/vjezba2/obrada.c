#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define N 6    /* broj razina proriteta */
#define LINUS_IS_WRONG

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;

int sig[]={SIGUSR1, SIGUSR2, SIGQUIT, SIGTERM, SIGINT};
int getIndex(int sigy){
    int i=0;
    for (;i<5;i++){
        if (sig[i] == sigy)
            return i + 1;
    }
    return -1;
}
void zabrani_prekidanje(int cur_priority){
   while(--cur_priority)
      sighold(sig[cur_priority]);
}
void dozvoli_prekidanje(int cur_priority){
   while(--cur_priority)
      sigrelse(sig[cur_priority]);
}

void obrada_signala(int i, int sleepTime){
    int j = 0;
    char out[] = "------\n";
    out[i] = 'P';
    printf("%s", out);
    for(;j<sleepTime;j++){
        sleep(1);
        out[i] = (char)('1' + j);
        printf("%s", out);
    }
    out[i] = 'K';
    printf("%s", out);
#ifdef LINUS_IS_WRONG
    OZNAKA_CEKANJA[i]--;
#endif
    return;
}
#ifndef LINUS_IS_WRONG
void prekidna_rutina(int sig){
    int n=-1;
    char out[] = "------\n";
    n = getIndex(sig);
    
    if (n == -1)
        return;
    
    zabrani_prekidanje(n);
    out[n] = 'X';
    printf("%s", out);
    obrada_signala(n, 5);
    dozvoli_prekidanje(n);
}
#else
void prekidna_rutina(int sig){
    int n=-1;
    char out[] = "------\n";
    n = getIndex(sig);
    
    if (n == -1)
        return;
    
    zabrani_prekidanje(n);
    out[n] = 'X';
    printf("%s", out);
    OZNAKA_CEKANJA[n]++;
    dozvoli_prekidanje(n);
}
#endif
int main ( void )
{
    int i=0;
    for (;i<5;i++){
        sigset (sig[i], prekidna_rutina);
    }
    sigset(SIGKILL, exit);
    printf("Proces obrade prekida, PID=%d \n", (int)getpid());

    printf ("Zavrsio osnovni program\n");
    
    while(1) {
#ifndef LINUS_IS_WRONG
        for (i = N - 1;i>=0;i--) {
            if (OZNAKA_CEKANJA[i] > 0) {
                obrada_signala(i, 5);
            }
        }
#else
        sleep(4);
#endif
    }
    return 0;
}