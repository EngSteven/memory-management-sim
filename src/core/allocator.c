#include <stddef.h>
#include "allocator.h"
#include "blocks.h"
#include "log.h"

/* Algoritmo actual (default: FIRST-FIT) */
static AllocAlgorithm current_algo = ALLOC_FIRST_FIT;

/* FIRST-FIT */
static Block *find_first_fit(size_t size) {
    Block *curr = blocks_first();

    while (curr) {
        if (curr->is_free && curr->size >= size) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/* BEST-FIT */
static Block *find_best_fit(size_t size) {
    Block *curr = blocks_first();
    Block *best = NULL;

    while (curr) {
        if (curr->is_free && curr->size >= size) {
            if (best == NULL || curr->size < best->size) {
                best = curr;
            }
        }
        curr = curr->next;
    }

    return best;
}

/* WORST-FIT */
static Block *find_worst_fit(size_t size) {
    Block *curr = blocks_first();
    Block *worst = NULL;

    while (curr) {
        if (curr->is_free && curr->size >= size) {
            if (worst == NULL || curr->size > worst->size) {
                worst = curr;
            }
        }
        curr = curr->next;
    }

    return worst;
}

void allocator_set_algorithm(AllocAlgorithm algo) {
    current_algo = algo;
    log_info("Algoritmo de asignación cambiado a %d", (int)algo);
}

Block *allocator_find_block(size_t size) {
    switch (current_algo) {
        case ALLOC_FIRST_FIT:
            return find_first_fit(size);

        case ALLOC_BEST_FIT:
            return find_best_fit(size);

        case ALLOC_WORST_FIT:
            return find_worst_fit(size);

        default:
            log_error("Algoritmo de asignación desconocido (%d)", (int)current_algo);
            return NULL;
    }
}
