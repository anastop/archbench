/**
 * @file
 * Tests the effect of memory hierarchy when processing 
 * working sets of different size and with different access
 * patterns.
 * The metric used is the number of processor cycles needed 
 * to access (read) a single element.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "access_pattern.h"
#include "util/tsc_x86_64.h"

int main(int argc, char **argv)
{
    unsigned int num_elements, min_elements, a, *rand_stream, *seq_stream;
    unsigned long ws_size, max_ws_size, num_traversals, max_traversals, i;
    struct l *seql, *randl, *head, *head2;
    tsctimer_t t;

    // Repeat until either the maximum ws size is reached or
    // num_traversals becomes zero
    max_ws_size = 1UL<<31;

    // This configuration gives a total of 8.5 billions element accesses
    max_traversals = 1UL<<20;
    min_elements = 1<<4;

    // Array sequential access
    fprintf(stderr, "Array of integers, sequential access\n");
    fprintf(stderr, "----------------------------------\n");
    fprintf(stderr, "size of accessed element: %lu\n", sizeof(unsigned int));
    num_elements = min_elements;
    num_traversals = max_traversals;
    ws_size = num_elements * sizeof(unsigned int);
    
    while ( ws_size < max_ws_size && num_traversals > 0 ) {
        timer_clear(&t);
        seq_stream = create_sequential_access_array(num_elements);
    
        timer_start(&t);
        i = 0;
        a = 0;
        while ( i++ < num_elements * num_traversals ) {
            a = seq_stream[a];
            __asm__ __volatile__ ("" :: "m" (seq_stream[a]), "r" (a) );
    
        }
        timer_stop(&t);
    
        fprintf(stderr, "Working_set_KB: %.2lf  "
                        "Cycles_per_element: %lf  "
                        "[elements:%d, traversals:%lu]\n",
                        (double)ws_size/1024, 
                        timer_total(&t) / (double)i,
                        num_elements, 
                        num_traversals);
        free(seq_stream);

        num_elements <<= 1;
        num_traversals >>= 1;
        ws_size = num_elements * sizeof(unsigned int);
    }
    fprintf(stderr, "\n\n");


    // Array random access
    fprintf(stderr, "Array of integers, random access\n");
    fprintf(stderr, "----------------------------------\n");
    fprintf(stderr, "size of accessed element: %lu\n", sizeof(unsigned int));
    num_elements = min_elements;
    num_traversals = max_traversals;
    ws_size = num_elements * sizeof(unsigned int);
    
    while ( ws_size < max_ws_size && num_traversals > 0 ) {
        timer_clear(&t);
        rand_stream = create_random_access_array(num_elements);
    
        timer_start(&t);
        i = 0;
        a = 0;
        while ( i++ < num_elements * num_traversals ) {
            a = rand_stream[a];
            __asm__ __volatile__ ("" :: "m" (rand_stream[a]), "r" (a) );
    
        }
        timer_stop(&t);
    
        fprintf(stderr, "Working_set_KB: %.2lf  "
                        "Cycles_per_element: %lf  "
                        "[elements:%d, traversals:%lu]\n",
                        (double)ws_size/1024, 
                        timer_total(&t) / (double)i,
                        num_elements, 
                        num_traversals);
        free(rand_stream);

        num_elements <<= 1;
        num_traversals >>= 1;
        ws_size = num_elements * sizeof(unsigned int);
    }
    fprintf(stderr, "\n\n");



    // Linked list sequential access
    fprintf(stderr, "Linked list, sequential access\n");
    fprintf(stderr, "----------------------------------\n");
    fprintf(stderr, "size of accessed element: %lu\n", sizeof(struct l*));
    fprintf(stderr, "size of struct: %lu\n", sizeof(struct l));
    num_elements = min_elements;
    num_traversals = max_traversals;
    ws_size = num_elements * sizeof(struct l*);
    
    while ( ws_size < max_ws_size && num_traversals > 0 ) {
        timer_clear(&t);
        seql = create_sequential_access_list(num_elements);
        head = seql;
    
        timer_start(&t);
        i = 0;
        a = 0;
        while ( i++ < num_elements * num_traversals ) {
            seql = seql->next;
            __asm__ __volatile__ ("" :: "m" (seql));
        }
        timer_stop(&t);
    
        fprintf(stderr, "Working_set_KB: %.2lf  "
                        "Cycles_per_element: %lf  "
                        "[elements:%d, traversals:%lu]\n",
                        (double)ws_size/1024, 
                        timer_total(&t) / (double)i,
                        num_elements, 
                        num_traversals);
        free(head);

        num_elements <<= 1;
        num_traversals >>= 1;
        ws_size = num_elements * sizeof(struct l*);
    }
    fprintf(stderr, "\n\n");



    // Linked list random access
    fprintf(stderr, "Linked list, random access\n");
    fprintf(stderr, "----------------------------------\n");
    fprintf(stderr, "size of accessed element: %lu\n", sizeof(struct l*));
    fprintf(stderr, "size of struct: %lu\n", sizeof(struct l));
    num_elements = min_elements;
    num_traversals = max_traversals;
    ws_size = num_elements * sizeof(struct l*);
    
    while ( ws_size < max_ws_size && num_traversals > 0 ) {
        timer_clear(&t);
        randl = create_random_access_list(num_elements);
        head2 = randl;
    
        timer_start(&t);
        i = 0;
        a = 0;
        while ( i++ < num_elements * num_traversals ) {
            randl = randl->next;
            __asm__ __volatile__ ("" :: "m" (randl));
        }
        timer_stop(&t);
    
        fprintf(stderr, "Working_set_KB: %.2lf  "
                        "Cycles_per_element: %lf  "
                        "[elements:%d, traversals:%lu]\n",
                        (double)ws_size/1024, 
                        timer_total(&t) / (double)i,
                        num_elements, 
                        num_traversals);
        free(head2);

        num_elements <<= 1;
        num_traversals >>= 1;
        ws_size = num_elements * sizeof(struct l*);
    }
    fprintf(stderr, "\n\n");


    // Loop overhead
    timer_clear(&t);
    timer_start(&t);
    i = 0;
    while ( i++ < (1UL<<29) ) {
        __asm__ __volatile__ ("" : : );
    }
    timer_stop(&t);
    fprintf(stderr, "Loop_cycles: %lf\n",
                    timer_total(&t) / (double)i);


    return 0;
}
