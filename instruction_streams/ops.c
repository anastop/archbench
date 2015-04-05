#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "memory_tests/access_pattern.h"
#include "util/tsc_x86_64.h"
#include "util/util.h"

#include "ops.h"
#include "threadinfo.h"


extern pthread_barrier_t barrier;
extern int exit_flag;

// number of elements of random array .
// adjust for reuse at a certain cache level
static const unsigned long num_elements = 1UL<<10;

// ------------- fp-add/mul streams --------------------
void* faddmul_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);

        timer_start(&ta->tim);
        REPEAT_2000(addmulf64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* faddmul_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0; 
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(addmulf64_ilp3());
        timer_stop(&ta->tim);

        /*ta->block_repetitions++;*/
        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* faddmul_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(addmulf64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}




// -------------------------- fp-add streams ---------------------------
void* fadd_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);

        timer_start(&ta->tim);
        REPEAT_2000(addf64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* fadd_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0; 
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(addf64_ilp3());
        timer_stop(&ta->tim);

        /*ta->block_repetitions++;*/
        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}

void* fadd_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(addf64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}





// -------------------------- fp-mul streams ---------------------------
void* fmul_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);

        timer_start(&ta->tim);
        REPEAT_2000(mulf64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* fmul_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0; 
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(mulf64_ilp3());
        timer_stop(&ta->tim);

        /*ta->block_repetitions++;*/
        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* fmul_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(mulf64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}




// -------------------------- fp-div streams ---------------------------
void* fdiv_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);

        timer_start(&ta->tim);
        REPEAT_2000(divf64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* fdiv_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0; 
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(divf64_ilp3());
        timer_stop(&ta->tim);

        /*ta->block_repetitions++;*/
        ta->total_ops += 32000;
    }

    pthread_exit(NULL);
    return 0;
}


void* fdiv_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    double val=1.0;

    fprintf(stderr, "Thread %d:%x\n", 
                     ta->tid, get_current_thread_mask());
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_xmm0(val);
        load_xmm1(val);
        load_xmm2(val);
        load_xmm3(val);
        load_xmm4(val);
        load_xmm5(val);
        load_xmm6(val);
        load_xmm7(val);
        load_xmm8(val);
        load_xmm9(val);
        load_xmm10(val);
        load_xmm11(val);
        load_xmm12(val);
        load_xmm13(val);
        load_xmm14(val);
        load_xmm15(val);
        
        timer_start(&ta->tim);
        REPEAT_2000(divf64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}

void* mem_rand_minilp(void *args)
{
    thread_args_t *ta = (thread_args_t*)args;
    
    unsigned int a, *rstream = create_random_access_array(num_elements);
    unsigned long i;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        
        timer_start(&ta->tim);
        i = 0;
        a = 0;
        for ( i = 0; i < 30000; i++ ) {
            a = rstream[a];
            __asm__ __volatile__ ("" :: "m" (rstream[a]), "r" (a) );
        }
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }
    
    free(rstream);
    pthread_exit(NULL);
    return 0;
}

void* mem_rand_medilp(void *args)
{
    thread_args_t *ta = (thread_args_t*)args;
    
    unsigned int a1, a2, a3, 
                 *rstream1 = create_random_access_array(num_elements),
                 *rstream2 = create_random_access_array(num_elements),
                 *rstream3 = create_random_access_array(num_elements);
    unsigned long i;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        
        timer_start(&ta->tim);
        i = 0;
        a1 = a2 = a3 = 0;
        for ( i = 0; i < 10000; i++ ) {
            a1 = rstream1[a1];
            __asm__ __volatile__ ("" :: "m" (rstream1[a1]), "r" (a1) );
            a2 = rstream2[a2];
            __asm__ __volatile__ ("" :: "m" (rstream2[a2]), "r" (a2) );
            a3 = rstream3[a3];
            __asm__ __volatile__ ("" :: "m" (rstream3[a3]), "r" (a3) );
        }
        timer_stop(&ta->tim);

        ta->total_ops += 10000;
    }
    
    free(rstream1);
    free(rstream2);
    free(rstream3);
    pthread_exit(NULL);
    return 0;
}

void* mem_rand_maxilp(void *args)
{
    thread_args_t *ta = (thread_args_t*)args;
    
    unsigned int a1, a2, a3, a4, a5, a6, 
                 *rstream1 = create_random_access_array(num_elements),
                 *rstream2 = create_random_access_array(num_elements),
                 *rstream3 = create_random_access_array(num_elements),
                 *rstream4 = create_random_access_array(num_elements),
                 *rstream5 = create_random_access_array(num_elements),
                 *rstream6 = create_random_access_array(num_elements);
    unsigned long i;
    
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        
        timer_start(&ta->tim);
        i = 0;
        a1 = a2 = a3 = a4 = a5 = a6 = 0;
        for ( i = 0; i < 5000; i++ ) {
            a1 = rstream1[a1];
            __asm__ __volatile__ ("" :: "m" (rstream1[a1]), "r" (a1) );
            a2 = rstream2[a2];
            __asm__ __volatile__ ("" :: "m" (rstream2[a2]), "r" (a2) );
            a3 = rstream3[a3];
            __asm__ __volatile__ ("" :: "m" (rstream3[a3]), "r" (a3) );
            a4 = rstream4[a4];
            __asm__ __volatile__ ("" :: "m" (rstream4[a4]), "r" (a4) );
            a5 = rstream5[a5];
            __asm__ __volatile__ ("" :: "m" (rstream5[a5]), "r" (a5) );
            a6 = rstream6[a6];
            __asm__ __volatile__ ("" :: "m" (rstream6[a6]), "r" (a6) );

        }
        timer_stop(&ta->tim);

        ta->total_ops += 5000;
    }
    
    free(rstream1);
    free(rstream2);
    free(rstream3);
    pthread_exit(NULL);
    return 0;
}

// -------------------------- int-add streams ---------------------------
void* iadd_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=2;
        
    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_rbx(val);
        load_rcx(val);
        load_rdi(val);
        load_rsi(val);
        load_r8(val);
        load_r9(val);
        load_r10(val);
        load_r11(val);
        load_r12(val);
        load_r13(val);
        load_r14(val);
        load_r15(val);

        timer_start(&ta->tim);
        load_rax(val);
        load_rdx(val);
        REPEAT_2000(addi64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 28000;
    }

    pthread_exit(NULL);
    return 0;
}

void* iadd_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=2;

    pthread_barrier_wait(&barrier);
        
    while ( !exit_flag ) {
        load_rbx(val);
        load_rcx(val);
        load_rdi(val);
        load_rsi(val);
        load_r8(val);
        load_r9(val);
        load_r10(val);
        load_r11(val);
        load_r12(val);
        load_r13(val);
        load_r14(val);
        load_r15(val);
        
        timer_start(&ta->tim);
        load_rax(val);
        load_rdx(val);
        REPEAT_2000(addi64_ilp3());
        timer_stop(&ta->tim);

        ta->total_ops += 28000;
    }

    pthread_exit(NULL);
    return 0;
}

void* iadd_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=2;

    pthread_barrier_wait(&barrier);
        
    while ( !exit_flag ) {
        load_rbx(val);
        load_rcx(val);
        load_rdi(val);
        load_rsi(val);
        load_r8(val);
        load_r9(val);
        load_r10(val);
        load_r11(val);
        load_r12(val);
        load_r13(val);
        load_r14(val);
        load_r15(val);
        
        timer_start(&ta->tim);
        load_rax(val);
        load_rdx(val);
        REPEAT_2000(addi64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 26000;
    }

    pthread_exit(NULL);
    return 0;
}

// -------------------------- int-mul streams ---------------------------
void* imul_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=1;

    pthread_barrier_wait(&barrier);
    while ( !exit_flag ) {
        
        timer_start(&ta->tim);
        load_rax(val);
        REPEAT_10000(muli64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 10000;
    }

    pthread_exit(NULL);
    return 0;
}

void* imul_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=2;
    int val1=1;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        load_rbx(val);
        load_rcx(val);
        
        timer_start(&ta->tim);
        load_rax(val1);
        REPEAT_10000(muli64_ilp2());
        timer_stop(&ta->tim);

        ta->total_ops += 20000;
    }

    pthread_exit(NULL);
    return 0;
}

void* imul_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val=2;
    int val1=1;

    pthread_barrier_wait(&barrier);
    
    while ( !exit_flag ) {
        load_rbx(val);
        load_rcx(val);
        load_rdi(val);
        load_rsi(val);
        load_r8(val);
        load_r9(val);
        
        timer_start(&ta->tim);
        load_rax(val1);
        REPEAT_5000(muli64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}

// -------------------------- int-div streams ---------------------------
void* idiv_minilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val2=2;
    int val1=1;
    int val0=0;

    pthread_barrier_wait(&barrier);
    
    // Re-init registers because rdtsc affect them

    while ( !exit_flag ) {
        timer_start(&ta->tim);
        load_rax(val2);
        load_rbx(val1);
        load_rdx(val0);
        REPEAT_10000(divi64_ilp1());
        timer_stop(&ta->tim);

        ta->total_ops += 10000;
    }

    pthread_exit(NULL);
    return 0;
}

void* idiv_medilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val2=2;
    int val1=1;
    int val0=0;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        timer_start(&ta->tim);
        load_rax(val2);
        load_rdx(val0);
        load_rbx(val1);
        load_rcx(val1);
        load_rsi(val1);
        REPEAT_10000(divi64_ilp3());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}

void* idiv_maxilp(void* args)
{
    thread_args_t *ta = (thread_args_t*)args;
    int val2=2;
    int val1=1;
    int val0=0;

    pthread_barrier_wait(&barrier);

    while ( !exit_flag ) {
        timer_start(&ta->tim);
        load_rax(val2);
        load_rdx(val0);
        load_rbx(val1);
        load_rcx(val1);
        load_rdi(val1);
        load_rsi(val1);
        load_r8(val1);
        load_r9(val1);
        REPEAT_5000(divi64_ilp6());
        timer_stop(&ta->tim);

        ta->total_ops += 30000;
    }

    pthread_exit(NULL);
    return 0;
}
