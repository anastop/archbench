#!/bin/bash




int_streams_minilp=(iadd_minilp imul_minilp idiv_minilp imem_seq_minilp)
int_streams_medilp=(iadd_medilp imul_medilp idiv_medilp imem_seq_medilp)
int_streams_maxilp=(iadd_maxilp imul_maxilp idiv_maxilp imem_seq_maxilp)
    
fp_streams_minilp=(fadd_minilp faddmul_minilp fmul_minilp fdiv_minilp fmem_seq_minilp)
fp_streams_medilp=(fadd_medilp faddmul_medilp fmul_medilp fdiv_medilp fmem_seq_medilp)
fp_streams_maxilp=(fadd_maxilp faddmul_maxilp fmul_maxilp fdiv_maxilp fmem_seq_maxilp)

Lint=${#int_streams_minilp[@]}
Lfp=${#fp_streams_minilp[@]}


timeout=16

# --- INTEGER STREAMS ---

# single streams minilp
for ((i=0; i < Lint; i++))
do
    ./streams --stream1=${int_streams_minilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams minilp
for ((i=0; i < Lint; i++))
do
    for ((j=i; j < Lint; j++))
    do
        ./streams --stream1=${int_streams_minilp[i]} --stream2=${int_streams_minilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_minilp[i]}, ${int_streams_minilp[j]}\)
        sleep 2
    done
done


# single streams medilp
for ((i=0; i < Lint; i++))
do
    ./streams --stream1=${int_streams_medilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams medilp
for ((i=0; i < Lint; i++))
do
    for ((j=i; j < Lint; j++))
    do
        ./streams --stream1=${int_streams_medilp[i]} --stream2=${int_streams_medilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_medilp[i]}, ${int_streams_medilp[j]}\)
        sleep 2
    done
done



# single streams maxilp
for ((i=0; i < Lint; i++))
do
    ./streams --stream1=${int_streams_maxilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams maxilp
for ((i=0; i < Lint; i++))
do
    for ((j=i; j < Lint; j++))
    do
        ./streams --stream1=${int_streams_maxilp[i]} --stream2=${int_streams_maxilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_maxilp[i]}, ${int_streams_maxilp[j]}\)
        sleep 2
    done
done




# --- FP STREAMS ---

# single streams minilp
for ((i=0; i < Lfp; i++))
do
    ./streams --stream1=${fp_streams_minilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams minilp
for ((i=0; i < Lfp; i++))
do
    for ((j=i; j < Lfp; j++))
    do
        ./streams --stream1=${fp_streams_minilp[i]} --stream2=${fp_streams_minilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${fp_streams_minilp[i]}, ${fp_streams_minilp[j]}\)
        sleep 2
    done
done



# single streams medilp
for ((i=0; i < Lfp; i++))
do
    ./streams --stream1=${fp_streams_medilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams medilp
for ((i=0; i < Lfp; i++))
do
    for ((j=i; j < Lfp; j++))
    do
        ./streams --stream1=${fp_streams_medilp[i]} --stream2=${fp_streams_medilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${fp_streams_medilp[i]}, ${fp_streams_medilp[j]}\)
        sleep 2
    done
done



# single streams maxilp
for ((i=0; i < Lfp; i++))
do
    ./streams --stream1=${fp_streams_maxilp[i]} --nthreads=1 --aff=SP --timeout=$timeout
done

# dual streams maxilp
for ((i=0; i < Lfp; i++))
do
    for ((j=i; j < Lfp; j++))
    do
        ./streams --stream1=${fp_streams_maxilp[i]} --stream2=${fp_streams_maxilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${fp_streams_maxilp[i]}, ${fp_streams_maxilp[j]}\)
        sleep 2
    done
done



# --- MIXED (INT+FP) STREAMS --- (minilp)

for ((i=0; i < Lint; i++))
do
    for ((j=0; j < Lfp; j++))
    do
        ./streams --stream1=${int_streams_minilp[i]} --stream2=${fp_streams_minilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_minilp[i]}, ${int_streams_minilp[j]}\)
        sleep 2
    done
done


# --- MIXED (INT+FP) STREAMS --- (medilp)

for ((i=0; i < Lint; i++))
do
    for ((j=0; j < Lfp; j++))
    do
        ./streams --stream1=${int_streams_medilp[i]} --stream2=${fp_streams_medilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_medilp[i]}, ${int_streams_medilp[j]}\)
        sleep 2
    done
done


# --- MIXED (INT+FP) STREAMS --- (maxilp)

for ((i=0; i < Lint; i++))
do
    for ((j=0; j < Lfp; j++))
    do
        ./streams --stream1=${int_streams_maxilp[i]} --stream2=${fp_streams_maxilp[j]} --nthreads=2 --aff=SP --timeout=$timeout
        #echo \(${int_streams_maxilp[i]}, ${int_streams_maxilp[j]}\)
        sleep 2
    done
done


