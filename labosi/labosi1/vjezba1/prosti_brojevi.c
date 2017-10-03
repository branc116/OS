#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

void glavna_funkcija ();

int main (void)
{
    glavna_funkcija();

    return 0;
}

int pauza = 0;
int broj = 1000000001;
int zadnji = 1000000001;

__sighandler_t* periodicki_ispis (int i) {
    printf("Zadnji prosti broj je: %d\n", zadnji);
}

__sighandler_t* postavi_pauzu (int i) {
    
    pauza = 1 - pauza;
    if (pauza == 1){
        printf("Pause started\n");
    }
    else{
        printf("Pause stopped\n");
    }
}

__sighandler_t* prekini (int i) {
    printf("Trenutni broj koji se provijerava je: %d\n", broj);
    periodicki_ispis(1);
    printf("Exiting\n");
    exit(0);
}
int max_clal(unsigned long n){
    int next_n = n;
    while(next_n * next_n > n)
    {
        next_n /= 2;
    }
    return next_n * next_n;
}
int prost ( unsigned long n ) {
    unsigned long i, max;

    if ( ( n & 1 ) == 0 ) /* je li paran? */
        return 0;

    max = max_clal(n);
    for ( i = 3; i <= max; i += 2 )
        if ( ( n % i ) == 0 )
            return 0;

    return 1; /* broj je prost! */
}
void glavna_funkcija () {
    struct itimerval t;
    /*povezi_signale_s_funkcijama; // na signal SIGTERM pozovi funkciju prekini() */
    
    sigset ( SIGTERM, prekini );
    sigset ( SIGINT, postavi_pauzu );
    sigset ( SIGALRM, periodicki_ispis );
    sigset ( SIGQUIT, prekini );

    t.it_value.tv_sec = 0;
    t.it_value.tv_usec = 500000;
    t.it_interval.tv_sec = 0;
    t.it_interval.tv_usec = 500000;

    setitimer ( ITIMER_REAL, &t, NULL );
    while(1) {
        
        if (pauza == 1){
            printf("Programim is no longer calculating\n");
            pause();
            printf("Program is continuing with calculations\n");
        }
        else{
            if ( prost ( broj ))
            zadnji = broj;
            broj++;
        }
    }
}
