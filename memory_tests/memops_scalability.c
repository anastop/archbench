/**
 * @file
 * Tests scalability of various memory operations on
 * multicores
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

unsigned int dum_var, dum_res;

int mask_small, mask_big;

/*
 * Working sets (with 32-bit elements)
 */

// per-thread arrays that fit LLC
unsigned int **priv_array_small; 
// per-thread arrays that do not fit LLC
unsigned int **priv_array_big; 
// shared array that fits LLC 
unsigned int *shrd_array_small; 
// shared array that does not fit LLC 
unsigned int *shrd_array_big; 

// shared variable aligned at cache line boundary
unsigned int *shrd_var_al;
// misaligned shared variable 
unsigned int *shrd_var_misal;
// per-thread variables aligned at cache line boundary
unsigned int **priv_var_al;
// per-thread misaligned variables
unsigned int **priv_var_misal;

/*
 * Generic operations specifications
 *
 * RD: read
 * WR: write
 * RMW: read-modify-write (e.g. add to memory location)
 *
 * PRIV: operation on thread-private data
 * SHRD: operation on shared data
 *
 * 1VAR: operation on a single variable
 * FIT_L1: operation on circular array that fits L1 cache
 * NOFIT_LLC: operation on circular array that does not fit LLC 
 *
 * MISAL: data accessed is misaligned (spans 2 cache lines)
 * ATOMIC: atomic access 
 */ 
typedef enum {
    NO_OP = 0, 
    RD_PRIV_1VAR,     
    RD_PRIV_1VAR_MISAL,
    RD_PRIV_FIT_L1, 
    RD_PRIV_NOFIT_LLC, 
    RD_SHRD_1VAR,     
    RD_SHRD_1VAR_MISAL,     
    RD_SHRD_FIT_L1, 
    RD_SHRD_NOFIT_LLC, 
    WR_PRIV_1VAR,     
    WR_PRIV_1VAR_MISAL,     
    WR_PRIV_FIT_L1, 
    WR_PRIV_NOFIT_LLC, 
    WR_SHRD_1VAR,     
    WR_SHRD_1VAR_MISAL,     
    WR_SHRD_FIT_L1, 
    WR_SHRD_NOFIT_LLC, 
    RMW_PRIV_1VAR,     
    RMW_PRIV_1VAR_MISAL,     
    RMW_PRIV_FIT_L1, 
    RMW_PRIV_NOFIT_LLC, 
    RMW_SHRD_1VAR,     
    RMW_SHRD_1VAR_MISAL,     
    RMW_SHRD_FIT_L1, 
    RMW_SHRD_NOFIT_LLC, 
    ATOMIC_RMW_PRIV_1VAR,     
    ATOMIC_RMW_PRIV_1VAR_MISAL,     
    ATOMIC_RMW_PRIV_FIT_L1, 
    ATOMIC_RMW_PRIV_NOFIT_LLC, 
    ATOMIC_RMW_SHRD_1VAR,     
    ATOMIC_RMW_SHRD_1VAR_MISAL,     
    ATOMIC_RMW_SHRD_FIT_L1, 
    ATOMIC_RMW_SHRD_NOFIT_LLC, 
    MFENCE_REG,
    MFENCE_WRMEM,
    MFENCE_RMWMEM_1VAR,
    MFENCE_RMWMEM_NOFIT_LLC,
    MFENCE_LOCKADD_REG,
    MFENCE_LOCKADD_WRMEM,
    MFENCE_LOCKADD_RMWMEM_1VAR,
    MFENCE_LOCKADD_RMWMEM_NOFIT_LLC,
    MARSAGLIA_PRNG
} opcode_t;

typedef struct {
    opcode_t code;
    char *name;
} op_desc_t;

#define INIT_OP(o) {.code = o, .name = #o}

op_desc_t ops[] = {
    INIT_OP(RD_PRIV_1VAR),    
    INIT_OP(RD_PRIV_1VAR_MISAL), 
    INIT_OP(RD_PRIV_FIT_L1),
    INIT_OP(RD_PRIV_NOFIT_LLC),
    INIT_OP(RD_SHRD_1VAR),   
    INIT_OP(RD_SHRD_1VAR_MISAL),  
    INIT_OP(RD_SHRD_FIT_L1),
    INIT_OP(RD_SHRD_NOFIT_LLC),
    INIT_OP(WR_PRIV_1VAR),   
    INIT_OP(WR_PRIV_1VAR_MISAL),  
    INIT_OP(WR_PRIV_FIT_L1),
    INIT_OP(WR_PRIV_NOFIT_LLC),
    INIT_OP(WR_SHRD_1VAR),   
    INIT_OP(WR_SHRD_1VAR_MISAL),  
    INIT_OP(WR_SHRD_FIT_L1),
    INIT_OP(WR_SHRD_NOFIT_LLC),
    INIT_OP(RMW_PRIV_1VAR),    
    INIT_OP(RMW_PRIV_1VAR_MISAL),  
    INIT_OP(RMW_PRIV_FIT_L1),
    INIT_OP(RMW_PRIV_NOFIT_LLC),
    INIT_OP(RMW_SHRD_1VAR),   
    INIT_OP(RMW_SHRD_1VAR_MISAL),  
    INIT_OP(RMW_SHRD_FIT_L1),
    INIT_OP(RMW_SHRD_NOFIT_LLC),
    INIT_OP(ATOMIC_RMW_PRIV_1VAR),   
    INIT_OP(ATOMIC_RMW_PRIV_1VAR_MISAL),  
    INIT_OP(ATOMIC_RMW_PRIV_FIT_L1),
    INIT_OP(ATOMIC_RMW_PRIV_NOFIT_LLC),
    INIT_OP(ATOMIC_RMW_SHRD_1VAR),   
    INIT_OP(ATOMIC_RMW_SHRD_1VAR_MISAL),  
    INIT_OP(ATOMIC_RMW_SHRD_FIT_L1),
    INIT_OP(ATOMIC_RMW_SHRD_NOFIT_LLC),
    INIT_OP(MFENCE_REG),
    INIT_OP(MFENCE_WRMEM),
    INIT_OP(MFENCE_RMWMEM_1VAR),
    INIT_OP(MFENCE_RMWMEM_NOFIT_LLC),
    INIT_OP(MFENCE_LOCKADD_REG),
    INIT_OP(MFENCE_LOCKADD_WRMEM),
    INIT_OP(MFENCE_LOCKADD_RMWMEM_1VAR),
    INIT_OP(MFENCE_LOCKADD_RMWMEM_NOFIT_LLC),
    INIT_OP(MARSAGLIA_PRNG),    
    INIT_OP(NO_OP)  
};

typedef struct {
    int id;
    op_desc_t *od;
} targs_t;


void* thread_fn(void *args)
{
    register unsigned int _x = 123456789, _y = 362436069,
                 _z = 521288629, _w = 88675123;
    register int _t;

    unsigned long i = 0;
    unsigned int x = 0;
    targs_t *ta = (targs_t*)args;

    unsigned int *my_array_small = priv_array_small[ta->id];
    unsigned int *my_array_big = priv_array_big[ta->id];
    unsigned int *my_var_al = priv_var_al[ta->id];
    unsigned int *my_var_misal = priv_var_misal[ta->id];
  
    pthread_barrier_wait(&bar);
    if ( ta->id == 0 ) timer_start(&tim);

    switch ( ta->od->code ) {
      
        case RD_PRIV_1VAR:
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=r" (x) 
                                      : "m" (*my_var_al));
            }
            break;
            
        case RD_PRIV_1VAR_MISAL:
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0"
                                      : "=r" (x)
                                      : "m" (*my_var_misal));
            }
            break;

        /*
         * NOTE: Without inserting an artificial data dependence (e.g. by 
         *       using the returned memory value to calculate next address)
         *       part of the loop body may be dynamically unrolled by the 
         *       processor. This will result in overlapping memory accesses
         *       of consecutive iterations. 
         *       But, isn't this scenario what we really want to measure? 
         */
        case RD_PRIV_FIT_L1:
            x = 0;
            while ( i++ < iters ) {
                /*_t = (_x ^ (_x << 11)) + x;*/
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                __asm__ __volatile__ ("movl %1, %0"
                                      : "=r" (x)
                                      : "m" (my_array_small[_w & mask_small]) );

            }
            break;

        case RD_PRIV_NOFIT_LLC:
            x = 0;
            while ( i++ < iters ) {
                /*_t = (_x ^ (_x << 11)) + x;*/
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("movl %1, %0"
                                      : "=r" (x)
                                      : "m" (my_array_big[_w & mask_big]) );
            }
            break;

        case RD_SHRD_1VAR:
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=r" (x) 
                                      : "m" (*shrd_var_al));
            }
            break;
            
        case RD_SHRD_1VAR_MISAL:
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0"
                                      : "=r" (x)
                                      : "m" (*shrd_var_misal));
            }
            break;


        case RD_SHRD_FIT_L1:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                __asm__ __volatile__ ("movl %1, %0"
                                      : "=r" (x)
                                      : "m" (shrd_array_small[_w & mask_small]) );

            }
            break;

        case RD_SHRD_NOFIT_LLC:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("movl %1, %0"
                                      : "=r" (x)
                                      : "m" (shrd_array_big[_w & mask_big]) );
            }
            break;

        case WR_PRIV_1VAR:
            x = 0;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case WR_PRIV_1VAR_MISAL:
            x = 0;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0"
                                      : "=m" (*my_var_misal) 
                                      : "r" (x) );
            }
            break;

        case WR_PRIV_FIT_L1:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                __asm__ __volatile__ ("movl %1, %0"
                                      : "=m" (my_array_small[_w & mask_small]) 
                                      : "r" (x) );               
            }
            break;

        case WR_PRIV_NOFIT_LLC:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("movl %1, %0"
                                      : "=m" (my_array_big[_w & mask_big])
                                      : "r" (x) );
            }
            break;

        case WR_SHRD_1VAR:
            x = 0;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case WR_SHRD_1VAR_MISAL:
            x = 0;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0"
                                      : "=m" (*shrd_var_misal) 
                                      : "r" (x) );
            }
            break;

        case WR_SHRD_FIT_L1:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
              
                __asm__ __volatile__ ("movl %1, %0"
                                      :"=m" (shrd_array_small[_w & mask_small])
                                      :"r" (x) );              
            }
            break;

        case WR_SHRD_NOFIT_LLC:
            x = 0;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("movl %1, %0"
                                      : "=m" (shrd_array_big[_w & mask_big])
                                      : "r" (x) );
            }
            break;

        case RMW_PRIV_1VAR:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
            }
            break;
             
        case RMW_PRIV_1VAR_MISAL:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0"
                                      : "=m" (*my_var_misal) 
                                      : "r" (x) );
            }
            break;

        case RMW_PRIV_FIT_L1:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                __asm__ __volatile__ ("addl %1, %0"
                                      : "=m" (my_array_small[_w & mask_small]) 
                                      : "r" (x) );               
            }
            break;

        case RMW_PRIV_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("addl %1, %0"
                                      : "=m" (my_array_big[_w & mask_big])
                                      : "r" (x) );
            }
            break;

        case RMW_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*shrd_var_al) 
                                      : "r" (x) );
            }
            break;
            
        case RMW_SHRD_1VAR_MISAL:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("addl %1,%0"
                                      : "=m" (*shrd_var_misal) 
                                      : "r" (x) );
            }
            break;

        case RMW_SHRD_FIT_L1:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                __asm__ __volatile__ ("addl %1, %0"
                                      :"=m" (shrd_array_small[_w & mask_small]) 
                                      :"r" (x) );               
            }
            break;

        case RMW_SHRD_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                __asm__ __volatile__ ("addl %1, %0"
                                      : "=m" (shrd_array_big[_w & mask_big])
                                      : "r" (x) );
            }
            break;

        case ATOMIC_RMW_PRIV_1VAR:
            x = 2;
            while ( i++ < iters ) {
                atomic_fetch_and_add_int(my_var_al, x);
            }
            break;
              
        case ATOMIC_RMW_PRIV_1VAR_MISAL:
            x = 2;
            while ( i++ < iters ) {
                atomic_fetch_and_add_int(my_var_misal, x);
            }
            break;

        case ATOMIC_RMW_PRIV_FIT_L1:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
               
                atomic_fetch_and_add_int(&my_array_small[_w & mask_small], x);
            }
            break;

        case ATOMIC_RMW_PRIV_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                atomic_fetch_and_add_int(&my_array_big[_w & mask_big], x);
            }
            break;

        case ATOMIC_RMW_SHRD_1VAR:
            x = 2;
            while ( i++ < iters ) {
                atomic_fetch_and_add_int(shrd_var_al, x);
            }
            break;
            
        case ATOMIC_RMW_SHRD_1VAR_MISAL:
            x = 2;
            while ( i++ < iters ) {
                atomic_fetch_and_add_int(shrd_var_misal, x);
            }
            break;

        case ATOMIC_RMW_SHRD_FIT_L1:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
                
                atomic_fetch_and_add_int(&shrd_array_small[_w & mask_small], x);
            }
            break;

        case ATOMIC_RMW_SHRD_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                atomic_fetch_and_add_int(&shrd_array_big[_w & mask_big], x);
            }
            break;

        case MFENCE_REG:
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                mfence();
            }
            break;

        case MFENCE_WRMEM:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
                mfence();
            }
            break;

        case MFENCE_RMWMEM_1VAR:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
                mfence();
            }
            break;

        case MFENCE_RMWMEM_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
                __asm__ __volatile__ ("addl %1, %0"
                                      : "=m" (my_array_big[_w & mask_big])
                                      : "r" (x) );
                mfence();
            }
            break;

        case MFENCE_LOCKADD_REG:
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));

                mfence_lockadd();
            }
            break;

        case MFENCE_LOCKADD_WRMEM:
            x = 2;
            while ( i++ < iters ) {
                __asm__ __volatile__ ("movl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
                mfence_lockadd();
            }
            break;

        case MFENCE_LOCKADD_RMWMEM_1VAR:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
                __asm__ __volatile__ ("addl %1,%0" 
                                      : "=m" (*my_var_al) 
                                      : "r" (x) );
                mfence_lockadd();
            }
            break;

        case MFENCE_LOCKADD_RMWMEM_NOFIT_LLC:
            x = 2;
            while ( i++ < iters ) {
                _t = (_x ^ (_x << 11));
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
                __asm__ __volatile__ ("addl %1, %0"
                                      : "=m" (my_array_big[_w & mask_big])
                                      : "r" (x) );
                mfence_lockadd();
            }
            break;

        case MARSAGLIA_PRNG:
            while ( i++ < iters ) {
                _t = _x ^ (_x << 11);
                _x = _y;  _y = _z;  _z = _w;
                _w = _w ^ (_w >> 19) ^ (_t ^ (_t >> 8));
            }
            dum_var = _w;
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
    int nbytes_small, nbytes_big, num_elements_small, num_elements_big,
        msb_pos, v;
 
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
    shrd_var_misal = (unsigned int*)(buffer+63);
    shrd_var_al = (unsigned int*)buffer;
    //printf("misal_var: %p\n", shrd_var_misal);
    //printf("al_var: %p\n", shrd_var_al);

    nbytes_small = 1<<14;   // 16 KB
    nbytes_big = 1<<27; // 128 MB
    
    num_elements_small = nbytes_small / sizeof(unsigned int);
    msb_pos = 0;
    v = num_elements_small;
    while ( v >>= 1 ) msb_pos++;
    mask_small = (1 << msb_pos) - 1;

    num_elements_big = nbytes_big / sizeof(unsigned int);
    msb_pos = 0;
    v = num_elements_big;
    while ( v >>= 1 ) msb_pos++;
    mask_big = (1 << msb_pos) - 1;

    fprintf(stdout, "Small array: %d elements, mask=%x\n", num_elements_small, 
                                                  mask_small);
    fprintf(stdout, "Big array: %d elements, mask=%x\n", num_elements_big, 
                                                  mask_big);

    shrd_array_small = malloc_safe(num_elements_small * sizeof(unsigned int));
    shrd_array_big = malloc_safe(num_elements_big * sizeof(unsigned int));

    // For all different thread numbers
    fprintf(stdout, "\n");
    for ( nthreads = 1; nthreads <= maxthreads; nthreads++ ) {

        fprintf(stdout, "Nthreads=%d\n", nthreads);
        fprintf(stdout, "==============\n");

        // allocate thread structures 
        tids = (pthread_t*)malloc_safe( nthreads * sizeof(pthread_t) );
        targs = (targs_t*)malloc_safe( nthreads * sizeof(targs_t)); 
        attr = (pthread_attr_t*)malloc_safe( nthreads * sizeof(pthread_attr_t)); 
        
        // allocate per-thread variables
        priv_array_small = (unsigned int**)malloc_safe(nthreads * 
                                                       sizeof(unsigned int*));
        priv_array_big = (unsigned int**)malloc_safe(nthreads * 
                                                     sizeof(unsigned int*));
        priv_var_al = (unsigned int**)malloc_safe(nthreads * 
                                                  sizeof(unsigned int*));
        priv_var_misal = (unsigned int**)malloc_safe(nthreads * 
                                                     sizeof(unsigned int*));

        // allocate per-thread arrays
        for ( i = 0; i < nthreads; i++ ) {
            priv_array_small[i] = malloc_safe(num_elements_small * sizeof(unsigned int)); 
            priv_array_big[i] = malloc_safe(num_elements_big * sizeof(unsigned int));
            memset(priv_array_small[i], 0, num_elements_small);
            memset(priv_array_big[i], 0, num_elements_big);

            char *base_addr = buffer + i*128;
            priv_var_al[i] = (unsigned int*)base_addr;
            priv_var_misal[i] = (unsigned int*)(base_addr+63);
        }

        pthread_barrier_init(&bar, NULL, nthreads);

        // for all different operations
        for ( op = 0; ; op++ ) {
            if ( ops[op].code == NO_OP ) break;
            fprintf(stdout, "\tnthreads:%d \top:%s ", 
                            nthreads,
                            ops[op].name);
         
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
        for ( i = 0; i < nthreads; i++ ) {
            free(priv_array_small[i]);
            free(priv_array_big[i]);
        }
        free(priv_array_small);
        free(priv_array_big);
        free(priv_var_al);
        free(priv_var_misal);
       
        dum_res += dum_var;
        fprintf(stdout, "\n");
    }

        
    fprintf(stdout, "%u\n", dum_res);


    // free(buffer) causes runtime error, because buffer does not
    // actually shows the beginning of memory returned by
    // malloc -- malloc_aligned should be modified to
    // provide a reference to original (unaligned) pointer 
    
    free(shrd_array_small);
    free(shrd_array_big);
    procmap_destroy(pi); 

    return 0;
}
