#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/*
 * Funciones relacionadas con la arena de memoria simulada.
 * Se implementan en src/core/memory.c
 */

void  memory_init(size_t size);
void  memory_destroy(void);
void *memory_arena(void);
void *memory_first_block(void);

#endif /* MEMORY_H */
