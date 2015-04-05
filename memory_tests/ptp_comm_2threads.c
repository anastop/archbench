/**
 * @file
 * Point-to-point communication benchmarks between 2 threads
 * on different processors
 */ 
#define _GNU_SOURCE

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "access_pattern.h"
#include "util/atomic_x86_64.h"
#include "util/tsc_x86_64.h"
#include "util/processor_map.h"
#include "util/util.h"

unsigned long iters;
pthread_barrier_t bar;
tsctimer_t tim;

// shared variable aligned at cache line boundary
unsigned int *shrd_var_al;

typedef enum {
    NO_OP = 0,
    RMW_SHRD_1VAR,
    ATOMIC_RMW_SHRD_1VAR
} opcode_t;

typedef struct {
    opcode_t code;
    char *name;
} op_desc_t;

#define INIT_OP(o) {.code = o, .name = #o}

op_desc_t ops[] = {
    INIT_OP(RMW_SHRD_1VAR),
    INIT_OP(ATOMIC_RMW_SHRD_1VAR),
    INIT_OP(NO_OP)  
};

typedef struct {
    int id;
    op_desc_t *od;
} targs_t;


void* thread_fn(void *args)
{
    unsigned long i = 0;
    unsigned int x = 2;
    targs_t *ta = (targs_t*)args;

    pthread_barrier_wait(&bar);
    if ( ta->id == 0 ) timer_start(&tim);

    switch ( ta->od->code ) {
        case RMW_SHRD_1VAR:
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case ATOMIC_RMW_SHRD_1VAR:
            while ( i++ < iters ) {
                atomic_fetch_and_add_int(shrd_var_al, x);
            }
            break;
            
        default:
            break;
    }

    pthread_barrier_wait(&bar);
    if ( ta->id == 0 ) timer_stop(&tim);

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    targs_t *targs;
    pthread_t *tids;
    pthread_attr_t *attr;
    procmap_t *pi;
    int p, c, i, nthreads, op, cpu_id;
  
    iters = atol(argv[1]);
    nthreads = 2;

    // Allocate shared variables
    char *buffer = (char*)malloc_aligned(4096, 64);
    shrd_var_al = (unsigned int*)buffer;

    pi = procmap_init();
    cpu_set_t cpusets[nthreads];

    // allocate thread structures 
    tids = (pthread_t*)malloc_safe( nthreads * sizeof(pthread_t) );
    targs = (targs_t*)malloc_safe( nthreads * sizeof(targs_t)); 
    attr = (pthread_attr_t*)malloc_safe( nthreads * 
                                         sizeof(pthread_attr_t)); 
    pthread_barrier_init(&bar, NULL, nthreads);

    // Set affinity for first thread
    // Place thread onto package 0, core 0, hw thread 0
    p = c = 0;
    cpu_id = pi->package[p].core[c].thread[0]->cpu_id;
    CPU_ZERO(&cpusets[0]);
    CPU_SET(cpu_id, &cpusets[0]);
            
    fprintf(stdout, "Thread_1 package:%d core:%d cpu_id:%d\n", 
                    p, c, cpu_id);

    // Iterate over different configurations for second
    // thread affinity.
    for ( p = 0; p < pi->num_packages; p++ ) {
        for ( c = 0; c < pi->num_cores_per_package; c++ ) {

            if ( p == 0 && c == 0 )
                continue;
            /*if ( p > 0 && c > 0 )*/
                /*continue;*/
            cpu_id = pi->package[p].core[c].thread[0]->cpu_id;
            CPU_ZERO(&cpusets[1]);
            CPU_SET(cpu_id, &cpusets[1]);
      
    
            // for all different operations
            for ( op = 0; ; op++ ) {
                if ( ops[op].code == NO_OP ) break;
                fprintf(stdout, "Thread_2 package:%d core:%d cpu_id:%d op:%s", 
                                p, c, cpu_id, ops[op].name);
                timer_clear(&tim);
                for ( i = 0; i < nthreads; i++ ) {
                    targs[i].id = i;
                    targs[i].od = &ops[op];
                    pthread_attr_init(&attr[i]);
                    pthread_attr_setaffinity_np(&attr[i], 
                                                sizeof(cpusets[i]), 
                                                &cpusets[i]);
                    pthread_create(&tids[i], 
                                   &attr[i], 
                                   thread_fn, 
                                   (void*)&targs[i]);
                }
                for ( i = 0; i < nthreads; i++ ) {
                    pthread_join(tids[i], NULL);
                    pthread_attr_destroy(&attr[i]);
                }
                fprintf(stdout, " cycles:%lf\n", 
                                timer_total(&tim) / (double)iters);
            }
            fprintf(stdout, "\n");
        }
    }

    pthread_barrier_destroy(&bar);
    free(tids);
    free(targs);
    free(attr);

    // free(buffer) causes runtime error, because buffer does not
    // actually shows the beginning of memory returned by
    // malloc -- malloc_aligned should be modified to
    // provide a reference to original (unaligned) pointer 
    
    procmap_destroy(pi); 

    return 0;
}
