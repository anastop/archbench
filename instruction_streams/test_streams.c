/**
 * @file
 * Evaluates co-execution of various instruction streams
 */ 

#define _GNU_SOURCE

#include <errno.h>
#include <getopt.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "util/tsc_x86_64.h"
#include "util/processor_map.h"
#include "util/util.h"

#include "threadinfo.h"
#include "ops.h"
/*#include "mem.h"*/

pthread_barrier_t barrier;

static int nthreads;
static int timeout_secs;
static thread_args_t *targs;

extern thread_method_t thr_methods[];
int exit_flag = 0;

static void signal_handler(int sig)
{
    int i;

    fprintf(stdout,"\n"); 
    for ( i = 0; i < nthreads; i++ ) 
        fprintf(stdout, 
                "thread:%d stream:%s CPI:%lf @ ", 
                targs[i].tid, 
                targs[i].stream->name, 
                timer_total(&targs[i].tim)/(double)targs[i].total_ops 
                );
    
    fprintf(stdout, "\n");
    fflush(stdout);

    exit_flag = 1;
}

static void setup_sigint_handler(void)
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;

    if( sigaction(SIGINT, &act, NULL) == -1 ) {
        perror("Unable to setup signal handler for SIGINT!\n");
        exit(EXIT_FAILURE);
    }
}

static void setup_sigalrm_handler(void)
{
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_handler;

    if ( sigaction(SIGALRM, &act, NULL) == -1 ) {
        perror("Unable to setup signal handler for SIGALRM!\n");
        exit(EXIT_FAILURE);
    }
}

static void fire_timer(int timeout_secs)
{
    struct itimerval timer;

    timer.it_value.tv_sec = timeout_secs;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;   //"one-shot" timer, no resets
    timer.it_interval.tv_usec = 0;      

    if( setitimer(ITIMER_REAL, &timer, NULL) == -1 ) {
        perror("Unable to setup timer!\n");
        exit(EXIT_FAILURE);
    }
}

static void print_usage(char *prog)
{
    fprintf(stdout, 
            "Usage: %s\n"
            " -s --streams=<stream list> \t#\":\" separated stream list\n"
            " -n --nthreads=<nth> \t#number of threads \n"
            " -t --timeout=<secs> \t#seconds before timeout\n"
            " -f --aff=[compact|scatter] \t#affinity policy\n"
            " -h --help \t\t#prints this help\n",
            prog);
    fprintf(stdout, "\n");
    fprintf(stdout, "Available instruction streams:\n");
    int i = 0;
    char *m;
    while ( ( m = thr_methods[i++].name ) ) 
        fprintf(stdout, "%s\n", m);
}


int main(int argc, char **argv)
{
    pthread_t *tids;
    procmap_t *pi;
    int p, c, t, i, next_option;
    
    char affinity_opt[32], streams_list[1024];
    const char* short_options = "s:n:hf:t:";
    const struct option long_options[]={
        {"streams", 1, NULL, 's'},
        {"nthreads", 1, NULL, 'n' },
        {"timeout", 1, NULL, 't' },
        {"help", 0, NULL, 'h' },
        {"aff", 1, NULL, 'f' },
        {NULL, 0, NULL, 0}
    };

    if ( argc == 1 ) {
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }
    
    do {
        next_option = getopt_long(argc, argv, short_options, long_options, NULL);

        switch(next_option) {
            case 's':
                sprintf(streams_list, "%s", optarg);
                break;

            case 'n':
                nthreads = atoi(optarg);  
                break;

            case 'f':
                sprintf(affinity_opt, "%s", optarg);
                break;
            
            case 't':
                timeout_secs = atoi(optarg);  
                break;

            case 'h':
                print_usage(argv[0]);
                break;

            case '?':
                fprintf(stderr, "Unknown option!\n");
                print_usage(argv[0]);
                exit(EXIT_FAILURE);

            case -1:    // Done with options
                break;  

            default:    // Unexpected error
                exit(EXIT_FAILURE);
        }

    } while(next_option != -1);

    targs = (thread_args_t*)malloc_safe( nthreads * sizeof(thread_args_t) );
    tids = (pthread_t*)malloc_safe( nthreads * sizeof(pthread_t) );

    i = 0; 
    char *token = strtok(streams_list, ":");
    while ( token != NULL ) {
        targs[i++].stream = get_thr_method_by_name(token, thr_methods);
        token = strtok(NULL, ":");
    }

    if ( i != nthreads ) {
        fprintf(stdout, 
                "Difference in size of streams list and number of threads."
                " Going to use first stream for all threads.");

        for ( i = 0; i < nthreads; i++ ) 
            targs[i].stream = targs[0].stream;
    }

    setup_sigint_handler();
    setup_sigalrm_handler();

    pi = procmap_init();
    cpu_set_t cpusets[pi->num_cpus];

    i = 0;
    fprintf(stdout, "Thread mapping:\n");
    
    if ( !strcmp(affinity_opt,"compact") ) {

        for ( p = 0; p < pi->num_packages; p++ ) {
            for ( c = 0; c < pi->num_cores_per_package; c++ ) {
                for ( t = 0; t < pi->num_threads_per_core; t++ ) {
                    int cpu_id = pi->package[p].core[c].thread[t]->cpu_id;
                    CPU_ZERO(&cpusets[i]);
                    CPU_SET(cpu_id, &cpusets[i]);

                    fprintf(stdout, "Thread %d @ package %d, core %d, "
                                    "hw thread %d (cpuid: %d)\n",
                                    i, p, c, t, cpu_id);
                    i++;
                }
            }    
        }
    } else if ( !strcmp(affinity_opt,"scatter") ) {

        for ( t = 0; t < pi->num_threads_per_core; t++ ) {
            for ( c = 0; c < pi->num_cores_per_package; c++ ) {
                for ( p = 0; p < pi->num_packages; p++ ) {
                    int cpu_id = pi->package[p].core[c].thread[t]->cpu_id;
                    CPU_ZERO(&cpusets[i]);
                    CPU_SET(cpu_id, &cpusets[i]);

                    fprintf(stdout, "Thread %d @ package %d, core %d, "
                                    "hw thread %d (cpuid: %d)\n",
                                    i, p, c, t, cpu_id);
                    i++;
                }
            }    
        }
    } else {
        fprintf(stderr, "Unknown option for affinity policy!\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    for ( i = 0; i < nthreads; i++ ) {
        targs[i].tid = i;
        targs[i].total_ops = 0;
        targs[i].block_repetitions = 0;
        timer_clear(&targs[i].tim);
        pthread_attr_init(&targs[i].attr);
        pthread_attr_setaffinity_np(&targs[i].attr,
                                    sizeof(cpu_set_t),
                                    &cpusets[i]);
    }
     
    pthread_barrier_init(&barrier, NULL, nthreads);

    fire_timer(timeout_secs);
     
    for(i=0; i<nthreads; i++) 
        pthread_create(&tids[i], 
                       &(targs[i].attr), 
                       targs[i].stream->function, 
                       (void*)&targs[i]);
    for(i=0; i<nthreads; i++)
        pthread_join(tids[i], NULL); 

    free(tids);
    free(targs);

    return 0;
}
