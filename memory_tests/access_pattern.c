/**
 * @file
 * Functions for implementing various memory access patterns
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/util.h"

/**
 * Permutes randomly an array. 
 * Uses Galois LFSR for randomization.
 * @param array array initial address
 * @param num_elements number of elements
 * @param elem_size number of bytes of each element
 */
static void fisher_yates_permute(void* array, unsigned int num_elements, 
                          size_t elem_size)
{
    unsigned int i,j;
    
    assert(elem_size > 0);
    
    for ( i = num_elements-1; i > 0; i-- ) {
        j = galois_lfsr() % i;
        swap(array+i*elem_size, array+j*elem_size, elem_size);
    }
}


/**
 * Permutes randomly an array of integers.
 * Uses galois LFSR for randomization.
 * @pre array must be initialized so that a[i] = i
 * @param a array
 * @param size array size
 */
static void fisher_yates_permute_integer(unsigned int* a, size_t size)
{
    int i,j;
    unsigned int tmp;

    for ( i = size-1; i > 0; i-- ) {
        j = galois_lfsr() % i;
        tmp = a[j]; 
        a[j] = a[i];
        a[i] = tmp;
    }
}


/**
 * Creates a cyclic array for indirectly accessing its elements 
 * sequentially. Each element s[i] shows the next element to access. 
 * @param size array size
 * @return pointer to array
 */
unsigned int* create_sequential_access_array(unsigned int size)
{
    unsigned int *s, i;

    s = (unsigned int*)malloc(size * sizeof(unsigned int));
    if ( !s ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }
    
    for ( i = 0; i < size-1; i+=64 ) 
        s[i] = i+64;

    s[i] = 0;

    return s;   
}



/**
 * Creates a cyclic array for indirectly accessing its elements in 
 * a random fashion. Each element s[i] shows the next element to access. 
 * @param size array size
 * @return pointer to array
 */
unsigned int* create_random_access_array(unsigned int size)
{
    unsigned int *s, i;

    s = (unsigned int*)malloc(size * sizeof(unsigned int));
    if ( !s ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    // necessary, in order to make accesses cyclic  
    for ( i = 0; i < size; i++ ) s[i] = i;

    fisher_yates_permute((void*)s, size, sizeof(unsigned int)); 

    // Make sure there is no element pointing to itself.
    // Since all elements have unique next-element values, the
    // previous problem degenerates to: is it possible for an 
    // element to be swapped back to its original position? 
    // The answer is no, because:
    // - all elements are being swapped
    // - after an element has been swapped to a position k, this
    // position is never considered again at a future swap
    for ( i = 0; i < size; i++ ) 
        assert(s[i] != i);

    return s;   
}



#define NPAD 7
struct l {
    struct l *next;
    long int pad[NPAD]; //!< payload
};

/**
 * Creates a cyclic linked list for pointer-jumping into
 * consecutive elements. 
 * @param size list size
 * @return pointer to first list element
 */ 
struct l* create_sequential_access_list(unsigned int size)
{
    unsigned int i;
    struct l *list_elements;
    
    list_elements = (struct l*)malloc(size * sizeof(struct l));
    if ( !list_elements ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    for ( i = 0; i < size-1; i++ ) {
        list_elements[i].next = &list_elements[i+1];
    }
    list_elements[i].next = &list_elements[0];

    return list_elements;
}


/**
 * Creates a cyclic linked list for pointer-jumping into
 * random elements. 
 * @param size list size
 * @return pointer to first list element
 */ 
struct l* create_random_access_list(unsigned int size)
{
    unsigned int i, *permut;
    struct l *list_elements;
    
    list_elements = (struct l*)malloc(size * sizeof(struct l));
    if ( !list_elements ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    // auxiliary array showing the permutation of the list elements
    permut = create_random_access_array(size); 

    for ( i = 0; i < size; i++ ) {
        list_elements[i].next = &list_elements[permut[i]];
    }

    free(permut);

    return list_elements;
}
