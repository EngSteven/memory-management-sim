#include <stdlib.h>
#include "blocks.h"
#include "log.h"

/*
 * Lista doblemente enlazada de bloques de memoria.
 */
static Block *first_block = NULL;

Block *block_create(size_t offset, size_t size, bool is_free) {
    Block *b = malloc(sizeof(Block));
    if (!b) {
        log_error("Error: malloc falló en block_create()");
        return NULL;
    }

    b->offset = offset;
    b->size = size;
    b->is_free = is_free;

    b->prev = NULL;
    b->next = NULL;

    // Si no hay bloques, este es el primero
    if (first_block == NULL) {
        first_block = b;
    }

    return b;
}

Block *blocks_first(void) {
    return first_block;
}

Block *blocks_next(Block *block) {
    if (!block) return NULL;
    return block->next;
}

/*
 * split() .
 */
void block_split(Block *block, size_t size) {
    // No split si el tamaño exacto o bloque ocupado
    if (block->size <= size) {
        return;
    }

    // Crear el bloque restante
    Block *rest = malloc(sizeof(Block));
    if (!rest) {
        log_error("Error: malloc falló en block_split()");
        return;
    }

    rest->offset = block->offset + size;
    rest->size   = block->size - size;
    rest->is_free = true;

    // Inserción en la lista
    rest->next = block->next;
    rest->prev = block;

    if (block->next) {
        block->next->prev = rest;
    }

    block->next = rest;

    // Reducir el bloque original al tamaño solicitado
    block->size = size;
}

Block *block_merge(Block *block) {
    if (!block) return NULL;

    Block *b = block;

    /* 1. Merge con prev */
    if (b->prev && b->prev->is_free && b->is_free) {
        Block *prev = b->prev;

        prev->size += b->size;
        prev->next  = b->next;

        if (b->next) {
            b->next->prev = prev;
        }

        free(b);
        b = prev;
    }

    /* 2. Merge con next */
    if (b->next && b->next->is_free && b->is_free) {
        Block *next = b->next;

        b->size += next->size;
        b->next  = next->next;

        if (next->next) {
            next->next->prev = b;
        }

        free(next);
    }

    return b;
}

void blocks_destroy(void) {
    Block *curr = first_block;
    while (curr) {
        Block *next = curr->next;
        free(curr);
        curr = next;
    }
    first_block = NULL;
}

