#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "blocks.h"
#include "log.h"

/*
 * Arena real de memoria simulada.
 */
static void *arena = NULL;
static size_t arena_size = 0;

/*
 * Inicializa la arena solicitando un bloque grande al sistema operativo.
 */
void memory_init(size_t size) {
    if (arena != NULL) {
        log_error("memory_init() llamado dos veces sin destroy()");
        return;
    }

    arena = malloc(size);
    if (!arena) {
        log_error("Error: no se pudo asignar arena de %zu bytes", size);
        exit(1);
    }

    arena_size = size;
    memset(arena, 0, size);

    // Crear bloque inicial libre
    Block *initial = block_create(0, size, true);

    if (!initial) {
        log_error("Error al crear bloque inicial");
        exit(1);
    }

    log_info("Arena inicializada (%zu bytes)", size);
}

/*
 * Libera la arena completa.
 */
void memory_destroy(void) {
    if (arena) {
        free(arena);
        arena = NULL;
        arena_size = 0;
    }

    // En FASE 3 agregaremos limpieza de lista de bloques
}

/*
 * Devuelve puntero bruto a la arena.
 */
void *memory_arena(void) {
    return arena;
}

/*
 * Devuelve el primer bloque de la lista enlazada de bloques.
 */
void *memory_first_block(void) {
    return blocks_first();
}
