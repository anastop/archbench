#include <string.h>

#include "threadinfo.h"

thread_method_t* get_thr_method_by_name(char *m, thread_method_t *methods)
{
    int i = 0;

    while ( 1 ) {
        if ( !methods[i].name || !strcmp(methods[i].name, m) )
            return &methods[i];
        i++;
    }
}

thread_method_t thr_methods[] = {
    INIT_THR_METHOD(fadd_minilp),
    INIT_THR_METHOD(fadd_medilp),
    INIT_THR_METHOD(fadd_maxilp),
    INIT_THR_METHOD(fmul_minilp),
    INIT_THR_METHOD(fmul_medilp),
    INIT_THR_METHOD(fmul_maxilp),
    INIT_THR_METHOD(faddmul_minilp),
    INIT_THR_METHOD(faddmul_medilp),
    INIT_THR_METHOD(faddmul_maxilp),
    INIT_THR_METHOD(fdiv_minilp),
    INIT_THR_METHOD(fdiv_medilp),
    INIT_THR_METHOD(fdiv_maxilp),
    INIT_THR_METHOD(iadd_minilp),
    INIT_THR_METHOD(iadd_medilp),
    INIT_THR_METHOD(iadd_maxilp),
    INIT_THR_METHOD(imul_minilp),
    INIT_THR_METHOD(imul_medilp),
    INIT_THR_METHOD(imul_maxilp),
    INIT_THR_METHOD(idiv_minilp),
    INIT_THR_METHOD(idiv_medilp),
    INIT_THR_METHOD(idiv_maxilp),
    INIT_THR_METHOD(mem_rand_minilp),
    INIT_THR_METHOD(mem_rand_medilp),
    INIT_THR_METHOD(mem_rand_maxilp),
    {.name = NULL, .function = NULL}
};
