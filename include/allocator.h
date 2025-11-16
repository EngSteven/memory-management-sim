#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include "blocks.h"

/*
 * Estrategias de asignación de memoria (first-fit, best-fit, worst-fit).
 */

typedef enum {
    ALLOC_FIRST_FIT,
    ALLOC_BEST_FIT,
    ALLOC_WORST_FIT
} AllocAlgorithm;

void  allocator_set_algorithm(AllocAlgorithm algo);
Block *allocator_find_block(size_t size);

#endif /* ALLOCATOR_H */
