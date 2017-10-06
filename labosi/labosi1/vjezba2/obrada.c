#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define N 6    /* broj razina proriteta */

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
    return;
}
void prekidna_rutina(int sig){
    int n=-1;
    char out[] = "------\n";
    n = getIndex(sig);
    
    if (n == -1)
        return;
    
    zabrani_prekidanje(n);
    out[n] = 'X';
    printf("%s", out);
    OZNAKA_CEKANJA[n]=1;
    obrada_signala(n, 5);
    dozvoli_prekidanje(n);
    /* // zabrani_prekidanje();
    // dozvoli_prekidanje(); */
}

int main ( void )
{
    int i=0;
    for (;i<5;i++){
        sigset (sig[i], prekidna_rutina);
    }
    sigset(SIGKILL, exit);
    printf("Proces obrade prekida, PID=%d \n", (int)getpid());

    printf ("Zavrsio osnovni program\n");
    obrada_signala(0, 12);
    return 0;
}