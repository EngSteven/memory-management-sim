#ifndef BLOCKS_H
#define BLOCKS_H

#include <stddef.h>
#include <stdbool.h>

/*
 * Representación de un bloque dentro de la arena.
 */

typedef struct Block {
    size_t offset;
    size_t size;
    bool   is_free;
    struct Block *next;
    struct Block *prev;
} Block;

Block *block_create(size_t offset, size_t size, bool is_free);
void   block_split(Block *block, size_t size);
Block *block_merge(Block *block);
Block *blocks_first(void);
Block *blocks_next(Block *block);
void blocks_destroy(void);

#endif /* BLOCKS_H */
