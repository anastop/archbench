#ifndef THREADINFO_H_
#define THREADINFO_H_

#include <pthread.h>

#include "tsc_x86_64.h"

typedef void* thread_func_t(void*);

typedef struct {
    char *name;
    thread_func_t *function;
} thread_method_t;

#define INIT_THR_METHOD(m) { .name = #m, .function = m }

typedef struct {
    int tid;
    pthread_attr_t attr;
    cpu_set_t cpuset;
    unsigned long total_ops;
    unsigned long block_repetitions;
    tsctimer_t tim;
    thread_method_t *stream;
} thread_args_t;

extern thread_method_t* get_thr_method_by_name(char *m, thread_method_t *avail_methods); 
extern thread_method_t thr_methods[];
extern thread_func_t    fadd_minilp,
                        fadd_medilp,
                        fadd_maxilp,
                        fmul_minilp,
                        fmul_medilp,
                        fmul_maxilp,
                        faddmul_minilp,
                        faddmul_medilp,
                        faddmul_maxilp,
                        fdiv_minilp,
                        fdiv_medilp,
                        fdiv_maxilp,
                        iadd_minilp,
                        iadd_medilp,
                        iadd_maxilp,
                        imul_minilp,
                        imul_medilp,
                        imul_maxilp,
                        idiv_minilp,
                        idiv_medilp,
                        idiv_maxilp,
                        mem_rand_minilp,
                        mem_rand_medilp,
                        mem_rand_maxilp;

#endif
