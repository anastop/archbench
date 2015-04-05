/**
 * @file
 * Function declarations and type definitions related to
 * memory access patterns
 */

#ifndef ACCESS_PATTERN_H_
#define ACCESS_PATTERN_H_

unsigned int* create_sequential_access_array(unsigned int size);
unsigned int* create_random_access_array(unsigned int size);

#define NPAD 0
struct l {
    struct l *next;
    long int pad[NPAD]; //!< payload
};

struct l* create_sequential_access_list(unsigned int size);
struct l* create_random_access_list(unsigned int size);


#endif // ACCESS_PATTERN_H_
