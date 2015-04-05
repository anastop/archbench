/**
 * @file
 * Tests scalability of various memory operations when 
 * intermixed with other operations
 */ 

#define _GNU_SOURCE

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

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
    WR_ONLY_SHRD_1VAR,     
    RMW_ONLY_SHRD_1VAR,     
    DELAY1,
    DELAY3,
    DELAY10,
    DELAY1_WR_SHRD_1VAR,
    DELAY3_WR_SHRD_1VAR,
    DELAY10_WR_SHRD_1VAR,
    DELAY1_RMW_SHRD_1VAR,
    DELAY3_RMW_SHRD_1VAR,
    DELAY10_RMW_SHRD_1VAR
} opcode_t;

typedef struct {
    opcode_t code;
    char *name;
} op_desc_t;

#define INIT_OP(o) {.code = o, .name = #o}

op_desc_t ops[] = {
    INIT_OP(DELAY1),
    INIT_OP(DELAY3),
    INIT_OP(DELAY10),
    INIT_OP(WR_ONLY_SHRD_1VAR),     
    INIT_OP(DELAY1_WR_SHRD_1VAR),     
    INIT_OP(DELAY3_WR_SHRD_1VAR),     
    INIT_OP(DELAY10_WR_SHRD_1VAR),     
    INIT_OP(RMW_ONLY_SHRD_1VAR),     
    INIT_OP(DELAY1_RMW_SHRD_1VAR),     
    INIT_OP(DELAY3_RMW_SHRD_1VAR),     
    INIT_OP(DELAY10_RMW_SHRD_1VAR),     
    INIT_OP(NO_OP)  
};

typedef struct {
    int id;
    op_desc_t *od;
} targs_t;

#define fp_work() {\
    __asm__ __volatile__ ( \
            "addsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm1,%%xmm2\n" \
            "addsd %%xmm2,%%xmm0\n" \
            ::: "xmm0","xmm1","xmm2"); }

#define int_work() {\
    __asm__ __volatile__ ( \
			"xorq %%rax,%%rbx\n" \
			"xorq %%rbx,%%rcx\n" \
			"xorq %%rcx,%%rdx\n" \
			"xorq %%rdx,%%rsi\n" \
			"xorq %%rsi,%%rdi\n" \
			"xorq %%rdi,%%r8\n" \
			"xorq %%r8,%%r9\n" \
			"xorq %%r9,%%r10\n" \
			"xorq %%r10,%%rax\n" \
            ::: "rax","rbx","rcx",\
                "rdx","rsi","rdi",\
                "r8","r9","r10" ); }

#define pause() { __asm__ __volatile__ ("pause"); }

#define delay() pause()

void* thread_fn(void *args)
{
    unsigned long i = 0;
    unsigned int x = 0;
    targs_t *ta = (targs_t*)args;

    pthread_barrier_wait(&bar);
    if ( ta->id == 0 ) timer_start(&tim);

    switch ( ta->od->code ) {
      
        case WR_ONLY_SHRD_1VAR:
            x = 0;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case RMW_ONLY_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
        
        case DELAY1:
            while ( i++ < iters ) {
                delay();
            }
            break;

        case DELAY3:
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
            }
            break;

        case DELAY10:
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
            }
            break;

        case DELAY1_WR_SHRD_1VAR:
            x = 0;
            while ( i++ < iters ) {
                delay();
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case DELAY3_WR_SHRD_1VAR:
            x = 0;
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case DELAY10_WR_SHRD_1VAR:
            x = 0;
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case DELAY1_RMW_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                delay();
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
           
        case DELAY3_RMW_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
           
        case DELAY10_RMW_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                delay();
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
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
    int p, c, t, i, nthreads, maxthreads, op;
    
    if ( argc < 3 ) {
       printf("Usage: ./prog <maxthreads> <iterations>\n");
       exit(EXIT_FAILURE);
    }
  
    maxthreads = atoi(argv[1]);
    iters = atol(argv[2]);

    pi = procmap_init();
    cpu_set_t cpusets[pi->num_cpus];

    // Configure thread affinity: first fill cores, then packages, 
    // and last peer threads
    i = 0;
    fprintf(stdout, "Thread mapping:\n");
    for ( t = 0; t < pi->num_threads_per_core; t++ ) {
        for ( p = 0; p < pi->num_packages; p++ ) {
            for ( c = 0; c < pi->num_cores_per_package; c++ ) {
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
    fprintf(stdout, "\n");

    // Allocate shared variables
    char *buffer = (char*)malloc_aligned(4096, 64);
    shrd_var_al = (unsigned int*)buffer;

    // For all different thread numbers
    fprintf(stdout, "\n");
    for ( nthreads = 1; nthreads <= maxthreads; nthreads++ ) {

        fprintf(stdout, "Nthreads=%d\n", nthreads);
        fprintf(stdout, "==============\n");

        // allocate thread structures 
        tids = (pthread_t*)malloc_safe( nthreads * sizeof(pthread_t) );
        targs = (targs_t*)malloc_safe( nthreads * sizeof(targs_t)); 
        attr = (pthread_attr_t*)malloc_safe( nthreads * sizeof(pthread_attr_t)); 
        pthread_barrier_init(&bar, NULL, nthreads);

        // for all different operations
        for ( op = 0; ; op++ ) {
            if ( ops[op].code == NO_OP ) break;
  
            fprintf(stdout, "\tnthreads:%d \top:%s ", 
                            nthreads, ops[op].name);
        
            timer_clear(&tim);

            for ( i = 0; i < nthreads; i++ ) {
                targs[i].id = i;
                targs[i].od = &ops[op];
                pthread_attr_init(&attr[i]);
                pthread_attr_setaffinity_np(&attr[i], 
                                            sizeof(cpusets[i]), 
                                            &cpusets[i]);
                pthread_create(&tids[i], &attr[i], thread_fn, (void*)&targs[i]);
            }
            for ( i = 0; i < nthreads; i++ ) {
                pthread_join(tids[i], NULL);
                pthread_attr_destroy(&attr[i]);
            }
    
            fprintf(stdout, "\tcycles:%lf\n", 
                            timer_total(&tim) / (double)iters);
        }
    
        pthread_barrier_destroy(&bar);
        free(tids);
        free(targs);
        free(attr);
       
        fprintf(stdout, "\n");
    }

    // free(buffer) causes runtime error, because buffer does not
    // actually shows the beginning of memory returned by
    // malloc -- malloc_aligned should be modified to
    // provide a reference to original (unaligned) pointer 
    
    procmap_destroy(pi); 

    return 0;
}
