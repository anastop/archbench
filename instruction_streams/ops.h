#ifndef OPS_H_
#define OPS_H_

#define REPEAT_2(x) x; x; 
#define REPEAT_5(x) x; x; x; x; x;  
#define REPEAT_10(x) x; x; x; x; x; x; x; x; x; x; 
#define REPEAT_100(x) REPEAT_10(REPEAT_10(x)) 
#define REPEAT_1000(x) REPEAT_100(REPEAT_10(x)) 
#define REPEAT_2000(x) REPEAT_1000(REPEAT_2(x)) 
#define REPEAT_5000(x) REPEAT_1000(REPEAT_5(x))   
#define REPEAT_10000(x) REPEAT_1000(REPEAT_10(x))  

/********* INTEGER MACROS **********/

// Moves

#define load_rax(x) { __asm__ __volatile__ ("movq %0,%%rax\n" ::"m"(x):"rax"); }
#define load_rbx(x) { __asm__ __volatile__ ("movq %0,%%rbx\n" ::"m"(x):"rbx"); }
#define load_rcx(x) { __asm__ __volatile__ ("movq %0,%%rcx\n" ::"m"(x):"rcx"); }
#define load_rdx(x) { __asm__ __volatile__ ("movq %0,%%rdx\n" ::"m"(x):"rdx"); }
#define load_rdi(x) { __asm__ __volatile__ ("movq %0,%%rdi\n" ::"m"(x):"rdi"); }
#define load_rbp(x) { __asm__ __volatile__ ("movq %0,%%rbp\n" ::"m"(x):"rbp"); }
#define load_rsi(x) { __asm__ __volatile__ ("movq %0,%%rsi\n" ::"m"(x):"rsi"); }
#define load_rsp(x) { __asm__ __volatile__ ("movq %0,%%rsp\n" ::"m"(x):"rsp"); }
#define load_r8(x) { __asm__ __volatile__ ("movq %0,%%r8\n" ::"m"(x):"r8"); }
#define load_r9(x) { __asm__ __volatile__ ("movq %0,%%r9\n" ::"m"(x):"r9"); }
#define load_r10(x) { __asm__ __volatile__ ("movq %0,%%r10\n" ::"m"(x):"r10"); }
#define load_r11(x) { __asm__ __volatile__ ("movq %0,%%r11\n" ::"m"(x):"r11"); }
#define load_r12(x) { __asm__ __volatile__ ("movq %0,%%r12\n" ::"m"(x):"r12"); }
#define load_r13(x) { __asm__ __volatile__ ("movq %0,%%r13\n" ::"m"(x):"r13"); }
#define load_r14(x) { __asm__ __volatile__ ("movq %0,%%r14\n" ::"m"(x):"r14"); }
#define load_r15(x) { __asm__ __volatile__ ("movq %0,%%r15\n" ::"m"(x):"r15"); }

#define store_rax(x) { __asm__ __volatile__ ("movq %%rax,%0\n" :"=m"(x):); }
#define store_rbx(x) { __asm__ __volatile__ ("movq %%rbx,%0\n" :"=m"(x):); }
#define store_rcx(x) { __asm__ __volatile__ ("movq %%rcx,%0\n" :"=m"(x):); }
#define store_rdx(x) { __asm__ __volatile__ ("movq %%rdx,%0\n" :"=m"(x):); }
#define store_rdi(x) { __asm__ __volatile__ ("movq %%rdi,%0\n" :"=m"(x):); }
#define store_rsi(x) { __asm__ __volatile__ ("movq %%rsi,%0\n" :"=m"(x):); }
#define store_rbp(x) { __asm__ __volatile__ ("movq %%rbp,%0\n" :"=m"(x):); }
#define store_rsp(x) { __asm__ __volatile__ ("movq %%rsp,%0\n" :"=m"(x):); }
#define store_r8(x) { __asm__ __volatile__ ("movq %%r8,%0\n" :"=m"(x):); }
#define store_r9(x) { __asm__ __volatile__ ("movq %%r9,%0\n" :"=m"(x):); }
#define store_r10(x) { __asm__ __volatile__ ("movq %%r10,%0\n" :"=m"(x):); }
#define store_r11(x) { __asm__ __volatile__ ("movq %%r11,%0\n" :"=m"(x):); }
#define store_r12(x) { __asm__ __volatile__ ("movq %%r12,%0\n" :"=m"(x):); }
#define store_r13(x) { __asm__ __volatile__ ("movq %%r13,%0\n" :"=m"(x):); }
#define store_r14(x) { __asm__ __volatile__ ("movq %%r14,%0\n" :"=m"(x):); }
#define store_r15(x) { __asm__ __volatile__ ("movq %%r15,%0\n" :"=m"(x):); }

// Addition

// 1 instr. chain
#define addi64_ilp1() { \
    __asm__ __volatile__ ( \
            "addq %%rax,%%rbx\n" \
            "addq %%rbx,%%rcx\n" \
            "addq %%rcx,%%rdx\n" \
            "addq %%rdx,%%rsi\n" \
            "addq %%rsi,%%rdi\n" \
            "addq %%rdi,%%r8\n" \
            "addq %%r8,%%r9\n" \
            "addq %%r9,%%r10\n" \
            "addq %%r10,%%r11\n" \
            "addq %%r11,%%r12\n" \
            "addq %%r12,%%r13\n" \
            "addq %%r13,%%r14\n" \
            "addq %%r14,%%r15\n" \
            "addq %%r15,%%rax\n" \
            : \
            : \
            :"rax","rbx","rcx","rdx","rdi","rsi", \
            "r8","r9","r10","r11","r12","r13","r14","r15" \
            ); \
}

// 3 instr. chains intermixed
#define addi64_ilp3() { \
    __asm__ __volatile__ ( \
            "addq %%rax,%%rbx\n" \
            "addq %%rsi,%%rdi\n" \
            "addq %%r11,%%r12\n" \
            "addq %%rbx,%%rcx\n" \
            "addq %%rdi,%%r8\n" \
            "addq %%r12,%%r13\n" \
            "addq %%rcx,%%rdx\n" \
            "addq %%r8,%%r9\n" \
            "addq %%r13,%%r14\n" \
            "addq %%rdx,%%rax\n" \
            "addq %%r9,%%r10\n" \
            "addq %%r14,%%r15\n" \
            "addq %%r10,%%rsi\n" \
            "addq %%r15,%%r11\n" \
            : \
            : \
            :"rax","rbx","rcx","rdx","rdi","rsi", \
            "r8","r9","r10","r11","r12","r13","r14","r15" \
            ); \
}

// >6 independent instructions
#define addi64_ilp6() { \
    __asm__ __volatile__ ( \
            "addq %%rax,%%rbx\n" \
            "addq %%rax,%%rcx\n" \
            "addq %%rax,%%rdx\n" \
            "addq %%rax,%%rdi\n" \
            "addq %%rax,%%rsi\n" \
            "addq %%rax,%%r8\n" \
            "addq %%rax,%%r9\n" \
            "addq %%rax,%%r10\n" \
            "addq %%rax,%%r11\n" \
            "addq %%rax,%%r12\n" \
            "addq %%rax,%%r13\n" \
            "addq %%rax,%%r14\n" \
            "addq %%rax,%%r15\n" \
            : \
            : \
            :"rax","rbx","rcx","rdx","rdi","rsi", \
            "r8","r9","r10","r11","r12","r13","r14","r15" \
            ); \
}


// Multiplications
// Implicitly, RDX:RAX = RAX * R_input

// rdx:rax = rax*rax
#define muli64_ilp1() { \
    __asm__ __volatile__ ("mulq %%rax\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}

// rdx:rax = rax*rax
// rdx:rax = rax*rbx
// (hoping that rdx:rax might eventually map to different 
// physical registers)
#define muli64_ilp2() { \
    __asm__ __volatile__ ( \
            "mulq %%rax\n" \
            "mulq %%rbx\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}

#define muli64_ilp6() { \
    __asm__ __volatile__ ( \
            "mulq %%rbx\n" \
            "mulq %%rcx\n" \
            "mulq %%rsi\n" \
            "mulq %%rdi\n" \
            "mulq %%r8\n" \
            "mulq %%r9\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}


// Divisions
// RDX:RAX = RDX:RAX / R_input

// rdx:rax = rdx:rax / rbx
#define divi64_ilp1() { \
    __asm__ __volatile__ ("divq %%rbx\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}

#define divi64_ilp3() { \
    __asm__ __volatile__ ( \
            "divq %%rbx\n" \
            "divq %%rcx\n" \
            "divq %%rsi\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}

#define divi64_ilp6() { \
    __asm__ __volatile__ ( \
            "divq %%rbx\n" \
            "divq %%rcx\n" \
            "divq %%r8\n" \
            "divq %%r9\n" \
            "divq %%rsi\n" \
            "divq %%rdi\n" \
            : \
            : \
            : "rax","rdx" \
            ); \
}


/********* FP MACROS **********/

#define load_xmm0(x) { __asm__ __volatile__ ("movsd %0,%%xmm0\n" : : "m"(x) : "xmm0"); }
#define load_xmm1(x) { __asm__ __volatile__ ("movsd %0,%%xmm1\n" : : "m"(x) : "xmm1"); }
#define load_xmm2(x) { __asm__ __volatile__ ("movsd %0,%%xmm2\n" : : "m"(x) : "xmm2"); }
#define load_xmm3(x) { __asm__ __volatile__ ("movsd %0,%%xmm3\n" : : "m"(x) : "xmm3"); }
#define load_xmm4(x) { __asm__ __volatile__ ("movsd %0,%%xmm4\n" : : "m"(x) : "xmm4"); }
#define load_xmm5(x) { __asm__ __volatile__ ("movsd %0,%%xmm5\n" : : "m"(x) : "xmm5"); }
#define load_xmm6(x) { __asm__ __volatile__ ("movsd %0,%%xmm6\n" : : "m"(x) : "xmm6"); }
#define load_xmm7(x) { __asm__ __volatile__ ("movsd %0,%%xmm7\n" : : "m"(x) : "xmm7"); }
#define load_xmm8(x) { __asm__ __volatile__ ("movsd %0,%%xmm8\n" : : "m"(x) : "xmm8"); }
#define load_xmm9(x) { __asm__ __volatile__ ("movsd %0,%%xmm9\n" : : "m"(x) : "xmm9"); }
#define load_xmm10(x) { __asm__ __volatile__ ("movsd %0,%%xmm10\n" : : "m"(x) : "xmm10"); }
#define load_xmm11(x) { __asm__ __volatile__ ("movsd %0,%%xmm11\n" : : "m"(x) : "xmm11"); }
#define load_xmm12(x) { __asm__ __volatile__ ("movsd %0,%%xmm12\n" : : "m"(x) : "xmm12"); }
#define load_xmm13(x) { __asm__ __volatile__ ("movsd %0,%%xmm13\n" : : "m"(x) : "xmm13"); }
#define load_xmm14(x) { __asm__ __volatile__ ("movsd %0,%%xmm14\n" : : "m"(x) : "xmm14"); }
#define load_xmm15(x) { __asm__ __volatile__ ("movsd %0,%%xmm15\n" : : "m"(x) : "xmm15"); }

#define store_xmm0(x) { __asm__ __volatile__ ("movsd %%xmm0,%0\n" : "=m"(x): ); }
#define store_xmm1(x) { __asm__ __volatile__ ("movsd %%xmm1,%0\n" : "=m"(x): ); }
#define store_xmm2(x) { __asm__ __volatile__ ("movsd %%xmm2,%0\n" : "=m"(x): ); }
#define store_xmm3(x) { __asm__ __volatile__ ("movsd %%xmm3,%0\n" : "=m"(x): ); }
#define store_xmm4(x) { __asm__ __volatile__ ("movsd %%xmm4,%0\n" : "=m"(x): ); }
#define store_xmm5(x) { __asm__ __volatile__ ("movsd %%xmm5,%0\n" : "=m"(x): ); }
#define store_xmm6(x) { __asm__ __volatile__ ("movsd %%xmm6,%0\n" : "=m"(x): ); }
#define store_xmm7(x) { __asm__ __volatile__ ("movsd %%xmm7,%0\n" : "=m"(x): ); }
#define store_xmm8(x) { __asm__ __volatile__ ("movsd %%xmm8,%0\n" : "=m"(x): ); }
#define store_xmm9(x) { __asm__ __volatile__ ("movsd %%xmm9,%0\n" : "=m"(x): ); }
#define store_xmm10(x) { __asm__ __volatile__ ("movsd %%xmm10,%0\n" : "=m"(x): ); }
#define store_xmm11(x) { __asm__ __volatile__ ("movsd %%xmm11,%0\n" : "=m"(x): ); }
#define store_xmm12(x) { __asm__ __volatile__ ("movsd %%xmm12,%0\n" : "=m"(x): ); }
#define store_xmm13(x) { __asm__ __volatile__ ("movsd %%xmm13,%0\n" : "=m"(x): ); }
#define store_xmm14(x) { __asm__ __volatile__ ("movsd %%xmm14,%0\n" : "=m"(x): ); }
#define store_xmm15(x) { __asm__ __volatile__ ("movsd %%xmm15,%0\n" : "=m"(x): ); }


// Addition

// 1 instr. chain
#define addf64_ilp1() { \
    __asm__ __volatile__ ( \
            "addsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm1,%%xmm2\n" \
            "addsd %%xmm2,%%xmm3\n" \
            "addsd %%xmm3,%%xmm4\n" \
            "addsd %%xmm4,%%xmm5\n" \
            "addsd %%xmm5,%%xmm6\n" \
            "addsd %%xmm6,%%xmm7\n" \
            "addsd %%xmm7,%%xmm8\n" \
            "addsd %%xmm8,%%xmm9\n" \
            "addsd %%xmm9,%%xmm10\n" \
            "addsd %%xmm10,%%xmm11\n" \
            "addsd %%xmm11,%%xmm12\n" \
            "addsd %%xmm12,%%xmm13\n" \
            "addsd %%xmm13,%%xmm14\n" \
            "addsd %%xmm14,%%xmm15\n" \
            "addsd %%xmm15,%%xmm0\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// 3 instr. chains intermixed
#define addf64_ilp3() { \
    __asm__ __volatile__ ( \
            "addsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm5,%%xmm6\n" \
            "addsd %%xmm10,%%xmm11\n" \
            "addsd %%xmm1,%%xmm2\n" \
            "addsd %%xmm6,%%xmm7\n" \
            "addsd %%xmm11,%%xmm12\n" \
            "addsd %%xmm2,%%xmm3\n" \
            "addsd %%xmm7,%%xmm8\n" \
            "addsd %%xmm12,%%xmm13\n" \
            "addsd %%xmm3,%%xmm4\n" \
            "addsd %%xmm8,%%xmm9\n" \
            "addsd %%xmm13,%%xmm14\n" \
            "addsd %%xmm4,%%xmm0\n" \
            "addsd %%xmm9,%%xmm5\n" \
            "addsd %%xmm14,%%xmm15\n" \
            "addsd %%xmm15,%%xmm10\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}


// >6 independent instructions
#define addf64_ilp6() { \
    __asm__ __volatile__ ( \
            "addsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm0,%%xmm2\n" \
            "addsd %%xmm0,%%xmm3\n" \
            "addsd %%xmm0,%%xmm4\n" \
            "addsd %%xmm0,%%xmm5\n" \
            "addsd %%xmm0,%%xmm6\n" \
            "addsd %%xmm0,%%xmm7\n" \
            "addsd %%xmm0,%%xmm8\n" \
            "addsd %%xmm0,%%xmm9\n" \
            "addsd %%xmm0,%%xmm10\n" \
            "addsd %%xmm0,%%xmm11\n" \
            "addsd %%xmm0,%%xmm12\n" \
            "addsd %%xmm0,%%xmm13\n" \
            "addsd %%xmm0,%%xmm14\n" \
            "addsd %%xmm0,%%xmm15\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}


//--Multiplication

// 1 instr. chain
#define mulf64_ilp1() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "mulsd %%xmm1,%%xmm2\n" \
            "mulsd %%xmm2,%%xmm3\n" \
            "mulsd %%xmm3,%%xmm4\n" \
            "mulsd %%xmm4,%%xmm5\n" \
            "mulsd %%xmm5,%%xmm6\n" \
            "mulsd %%xmm6,%%xmm7\n" \
            "mulsd %%xmm7,%%xmm8\n" \
            "mulsd %%xmm8,%%xmm9\n" \
            "mulsd %%xmm9,%%xmm10\n" \
            "mulsd %%xmm10,%%xmm11\n" \
            "mulsd %%xmm11,%%xmm12\n" \
            "mulsd %%xmm12,%%xmm13\n" \
            "mulsd %%xmm13,%%xmm14\n" \
            "mulsd %%xmm14,%%xmm15\n" \
            "mulsd %%xmm15,%%xmm0\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// 3 instr. chains intermixed
#define mulf64_ilp3() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "mulsd %%xmm5,%%xmm6\n" \
            "mulsd %%xmm10,%%xmm11\n" \
            "mulsd %%xmm1,%%xmm2\n" \
            "mulsd %%xmm6,%%xmm7\n" \
            "mulsd %%xmm11,%%xmm12\n" \
            "mulsd %%xmm2,%%xmm3\n" \
            "mulsd %%xmm7,%%xmm8\n" \
            "mulsd %%xmm12,%%xmm13\n" \
            "mulsd %%xmm3,%%xmm4\n" \
            "mulsd %%xmm8,%%xmm9\n" \
            "mulsd %%xmm13,%%xmm14\n" \
            "mulsd %%xmm4,%%xmm0\n" \
            "mulsd %%xmm9,%%xmm5\n" \
            "mulsd %%xmm14,%%xmm15\n" \
            "mulsd %%xmm15,%%xmm10\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// >6 independent instructions
#define mulf64_ilp6() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "mulsd %%xmm0,%%xmm2\n" \
            "mulsd %%xmm0,%%xmm3\n" \
            "mulsd %%xmm0,%%xmm4\n" \
            "mulsd %%xmm0,%%xmm5\n" \
            "mulsd %%xmm0,%%xmm6\n" \
            "mulsd %%xmm0,%%xmm7\n" \
            "mulsd %%xmm0,%%xmm8\n" \
            "mulsd %%xmm0,%%xmm9\n" \
            "mulsd %%xmm0,%%xmm10\n" \
            "mulsd %%xmm0,%%xmm11\n" \
            "mulsd %%xmm0,%%xmm12\n" \
            "mulsd %%xmm0,%%xmm13\n" \
            "mulsd %%xmm0,%%xmm14\n" \
            "mulsd %%xmm0,%%xmm15\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}


// Divisions

// 1 instr. chain
#define divf64_ilp1() { \
    __asm__ __volatile__ ( \
            "divsd %%xmm0,%%xmm1\n" \
            "divsd %%xmm1,%%xmm2\n" \
            "divsd %%xmm2,%%xmm3\n" \
            "divsd %%xmm3,%%xmm4\n" \
            "divsd %%xmm4,%%xmm5\n" \
            "divsd %%xmm5,%%xmm6\n" \
            "divsd %%xmm6,%%xmm7\n" \
            "divsd %%xmm7,%%xmm8\n" \
            "divsd %%xmm8,%%xmm9\n" \
            "divsd %%xmm9,%%xmm10\n" \
            "divsd %%xmm10,%%xmm11\n" \
            "divsd %%xmm11,%%xmm12\n" \
            "divsd %%xmm12,%%xmm13\n" \
            "divsd %%xmm13,%%xmm14\n" \
            "divsd %%xmm14,%%xmm15\n" \
            "divsd %%xmm15,%%xmm0\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// 3 instr. chains intermixed
#define divf64_ilp3() { \
    __asm__ __volatile__ ( \
            "divsd %%xmm0,%%xmm1\n" \
            "divsd %%xmm5,%%xmm6\n" \
            "divsd %%xmm10,%%xmm11\n" \
            "divsd %%xmm1,%%xmm2\n" \
            "divsd %%xmm6,%%xmm7\n" \
            "divsd %%xmm11,%%xmm12\n" \
            "divsd %%xmm2,%%xmm3\n" \
            "divsd %%xmm7,%%xmm8\n" \
            "divsd %%xmm12,%%xmm13\n" \
            "divsd %%xmm3,%%xmm4\n" \
            "divsd %%xmm8,%%xmm9\n" \
            "divsd %%xmm13,%%xmm14\n" \
            "divsd %%xmm4,%%xmm0\n" \
            "divsd %%xmm9,%%xmm5\n" \
            "divsd %%xmm14,%%xmm15\n" \
            "divsd %%xmm15,%%xmm10\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// >6 independent instructions
#define divf64_ilp6() { \
    __asm__ __volatile__ ( \
            "divsd %%xmm0,%%xmm1\n" \
            "divsd %%xmm0,%%xmm2\n" \
            "divsd %%xmm0,%%xmm3\n" \
            "divsd %%xmm0,%%xmm4\n" \
            "divsd %%xmm0,%%xmm5\n" \
            "divsd %%xmm0,%%xmm6\n" \
            "divsd %%xmm0,%%xmm7\n" \
            "divsd %%xmm0,%%xmm8\n" \
            "divsd %%xmm0,%%xmm9\n" \
            "divsd %%xmm0,%%xmm10\n" \
            "divsd %%xmm0,%%xmm11\n" \
            "divsd %%xmm0,%%xmm12\n" \
            "divsd %%xmm0,%%xmm13\n" \
            "divsd %%xmm0,%%xmm14\n" \
            "divsd %%xmm0,%%xmm15\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}


// Addition and Multiplication

// 1 instr. chain
#define addmulf64_ilp1() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm1,%%xmm2\n" \
            "mulsd %%xmm2,%%xmm3\n" \
            "addsd %%xmm3,%%xmm4\n" \
            "mulsd %%xmm4,%%xmm5\n" \
            "addsd %%xmm5,%%xmm6\n" \
            "mulsd %%xmm6,%%xmm7\n" \
            "addsd %%xmm7,%%xmm8\n" \
            "mulsd %%xmm8,%%xmm9\n" \
            "addsd %%xmm9,%%xmm10\n" \
            "mulsd %%xmm10,%%xmm11\n" \
            "addsd %%xmm11,%%xmm12\n" \
            "mulsd %%xmm12,%%xmm13\n" \
            "addsd %%xmm13,%%xmm14\n" \
            "mulsd %%xmm14,%%xmm15\n" \
            "addsd %%xmm15,%%xmm0\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// 3 instr. chains intermixed
#define addmulf64_ilp3() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm5,%%xmm6\n" \
            "mulsd %%xmm10,%%xmm11\n" \
            "addsd %%xmm1,%%xmm2\n" \
            "mulsd %%xmm6,%%xmm7\n" \
            "addsd %%xmm11,%%xmm12\n" \
            "mulsd %%xmm2,%%xmm3\n" \
            "addsd %%xmm7,%%xmm8\n" \
            "mulsd %%xmm12,%%xmm13\n" \
            "addsd %%xmm3,%%xmm4\n" \
            "mulsd %%xmm8,%%xmm9\n" \
            "addsd %%xmm13,%%xmm14\n" \
            "mulsd %%xmm4,%%xmm0\n" \
            "addsd %%xmm9,%%xmm5\n" \
            "mulsd %%xmm14,%%xmm15\n" \
            "addsd %%xmm15,%%xmm10\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

// >6 independent instructions
#define addmulf64_ilp6() { \
    __asm__ __volatile__ ( \
            "mulsd %%xmm0,%%xmm1\n" \
            "addsd %%xmm0,%%xmm2\n" \
            "mulsd %%xmm0,%%xmm3\n" \
            "addsd %%xmm0,%%xmm4\n" \
            "mulsd %%xmm0,%%xmm5\n" \
            "addsd %%xmm0,%%xmm6\n" \
            "mulsd %%xmm0,%%xmm7\n" \
            "addsd %%xmm0,%%xmm8\n" \
            "mulsd %%xmm0,%%xmm9\n" \
            "addsd %%xmm0,%%xmm10\n" \
            "mulsd %%xmm0,%%xmm11\n" \
            "addsd %%xmm0,%%xmm12\n" \
            "mulsd %%xmm0,%%xmm13\n" \
            "addsd %%xmm0,%%xmm14\n" \
            "mulsd %%xmm0,%%xmm15\n" \
            : \
            : \
            :"xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7", \
            "xmm8","xmm9","xmm10","xmm11","xmm12","xmm13","xmm14","xmm15" \
            ); \
}

#endif
