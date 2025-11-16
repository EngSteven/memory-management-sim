#include <string.h>
#include "memory_ops.h"
#include "allocator.h"
#include "blocks.h"
#include "variables.h"
#include "memory.h"
#include "log.h"
#include <stdlib.h>   

int mem_alloc(const char *name, size_t size) {
    // 1. Validar duplicado
    if (var_get(name) != NULL) {
        log_error("ALLOC: variable '%s' ya existe", name);
        return -1;
    }

    // 2. Buscar bloque libre con el algoritmo actual
    Block *block = allocator_find_block(size);
    if (!block) {
        log_error("ALLOC: no hay bloque libre suficiente para '%s' (%zu bytes)", name, size);
        return -1;
    }

    // 3. Split si el bloque es más grande de lo necesario
    if (block->size > size) {
        block_split(block, size);
    }

    // 4. Marcar bloque como ocupado
    block->is_free = false;

    // 5. Registrar variable
    var_set(name, block);

    // 6. Rellenar la arena con el nombre de la variable
    unsigned char *arena = (unsigned char *)memory_arena();
    for (size_t i = 0; i < size; i++) {
        arena[block->offset + i] = name[0]; // usamos solo la primera letra
    }

    log_info("ALLOC '%s' (%zu bytes) en offset=%zu", name, size, block->offset);
    return 0;
}

int mem_free(const char *name) {
    // 1. Obtener bloque asociado
    Block *b = var_get(name);
    if (!b) {
        log_error("FREE: variable '%s' no existe", name);
        return -1;
    }

    // 2. Marcar bloque libre
    b->is_free = true;

    // 3. Merge con vecinos libres
    block_merge(b);

    // 4. Eliminar variable de la tabla
    var_remove(name);

    log_info("FREE '%s'", name);
    return 0;
}

int mem_realloc(const char *name, size_t new_size) {
    Block *old = var_get(name);
    if (!old) {
        log_error("REALLOC: variable '%s' no existe", name);
        return -1;
    }

    size_t old_size = old->size;

    // Caso 0: new_size == 0 → FREE
    if (new_size == 0) {
        return mem_free(name);
    }

    // Caso 1: tamaños iguales
    if (new_size == old_size) {
        return 0;
    }

    unsigned char *arena = memory_arena();

    // Caso 2: reducción
    if (new_size < old_size) {
        block_split(old, new_size);
        block_merge(old); // por si se puede fusionar la parte sobrante
        log_info("REALLOC (reduce) '%s' %zu -> %zu bytes", name, old_size, new_size);
        return 0;
    }

    // Caso 3: expandir in-place
    size_t extra = new_size - old_size;

    /* Intentar coalescer el espacio libre inmediatamente después */
    if (old->next && old->next->is_free) {
        block_merge(old->next);  // fusiona old->next con sus vecinos libres (si hay)
    }

    /* Re-evaluar vecino inmediato */
    if (old->next && old->next->is_free && old->next->size >= extra) {
        Block *next = old->next;

        if (next->size == extra) {
            // Tomamos todo el bloque next → se elimina
            old->size += next->size;

            old->next = next->next;
            if (next->next) {
                next->next->prev = old;
            }
            free(next);
        } else {
            // Tomamos solo una parte de next
            next->offset += extra;
            next->size   -= extra;
            old->size    += extra;
        }

        // Rellenar la nueva parte
        for (size_t i = old_size; i < new_size; i++) {
            arena[old->offset + i] = name[0];
        }

        log_info("REALLOC (expand in-place) '%s' %zu -> %zu bytes", name, old_size, new_size);
        return 0;
    }

    // Caso 4: mover a un nuevo bloque
    Block *new_block = allocator_find_block(new_size);
    if (!new_block) {
        log_error("REALLOC: no hay bloque nuevo suficiente para '%s'", name);
        return -1;
    }

    // Split si sobra espacio
    if (new_block->size > new_size) {
        block_split(new_block, new_size);
    }

    new_block->is_free = false;

    // Copiar datos antiguos al nuevo bloque
    memcpy(arena + new_block->offset,
           arena + old->offset,
           old_size);

    // Rellenar resto
    for (size_t i = old_size; i < new_size; i++) {
        arena[new_block->offset + i] = name[0];
    }

    // Liberar bloque viejo
    mem_free(name);

    // Registrar el nuevo bloque
    var_set(name, new_block);

    log_info("REALLOC (move) '%s' %zu -> %zu bytes", name, old_size, new_size);
    return 0;
}
