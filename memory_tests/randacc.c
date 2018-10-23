/**
 * @file
 */

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "access_pattern.h"
#include "util/tsc_x86_64.h"

static volatile unsigned long i = 0;

static unsigned int interval_msecs = 500;
static tsctimer_t t;

static void sigalrm_hnd(int sig) 
{
    timer_stop(&t);
    fprintf(stderr, "elements_processed:%ld, cycles_per_element:%lf, ", 
                    i, timer_total(&t)/(double)i);
    i = 0;
    timer_clear(&t);
    timer_start(&t);
}

int main(int argc, char **argv)
{
    unsigned int ws_mb, num_elements, a, *rand_stream;
    struct sigaction sa;
    struct itimerval itv;
        
    timer_clear(&t);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrm_hnd;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = interval_msecs*1000;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = interval_msecs*1000;

    if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
        perror("setitimer");    
        exit(EXIT_FAILURE);
    }
    
    ws_mb = atoi(argv[1]);
    num_elements = ws_mb * 1024*1024/sizeof(unsigned int);

    rand_stream = create_random_access_array(num_elements);

    timer_start(&t);
    a = 0;
    for (;;) {
        i++;
        if (i==1) {
            fprintf(stderr, "reset\n");
        }
        a = rand_stream[a];
        __asm__ __volatile__ ("" :: "m" (rand_stream[a]), "r" (a) );
    }
    free(rand_stream);

    return 0;
}
