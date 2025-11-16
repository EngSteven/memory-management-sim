/**
 * @file memory.c
 * @brief Administración de la arena de memoria simulada.
 *
 * Este módulo gestiona la creación, destrucción y acceso a la arena principal
 * de memoria utilizada por el simulador. La arena consiste en un único bloque
 * grande solicitado al sistema operativo mediante `malloc()`. A partir de este
 * bloque se administran estructuras lógicas de bloques mediante la lista
 * implementada en `blocks.c`.
 *
 * Responsabilidades principales:
 *  - Inicializar la arena de memoria.
 *  - Destruir y liberar la arena.
 *  - Proveer punteros crudos a la memoria.
 *  - Proveer acceso al primer bloque lógico de la lista de bloques.
 */

#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "blocks.h"
#include "log.h"

/**
 * @brief Puntero a la arena real de memoria simulada.
 *
 * Este bloque grande es solicitado al sistema operativo solo una vez al inicio
 * mediante `malloc()` y posteriormente simulado como si fuera nuestro "heap".
 */
static void *arena = NULL;

/**
 * @brief Tamaño total de la arena de memoria en bytes.
 */
static size_t arena_size = 0;

/**
 * @brief Inicializa la arena de memoria del simulador.
 *
 * Solicita un bloque grande al sistema operativo, lo rellena con ceros y crea
 * el bloque inicial libre que representa toda la memoria disponible.  
 *  
 * Esta función **solo debe llamarse una vez** por ejecución. Si se invoca más
 * de una vez sin haber llamado antes a `memory_destroy()`, se registra un error.
 *
 * @param size Tamaño total (en bytes) que tendrá la arena simulada.
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

/**
 * @brief Libera completamente la arena de memoria.
 *
 * Esta función libera el bloque asignado por `memory_init()`, restablece
 * punteros y tamaños, y prepara al módulo para un uso futuro.
 *
 * Nota: La liberación de la lista de bloques se implementará en fases posteriores.
 */
void memory_destroy(void) {
    if (arena) {
        free(arena);
        arena = NULL;
        arena_size = 0;
    }

    // En FASE 3 agregaremos limpieza de lista de bloques
}

/**
 * @brief Devuelve el puntero crudo al inicio de la arena simulada.
 *
 * @return Puntero a la arena, o NULL si no ha sido inicializada.
 */
void *memory_arena(void) {
    return arena;
}

/**
 * @brief Retorna el primer bloque lógico de la lista de bloques.
 *
 * Los bloques representan fragmentos libres u ocupados dentro de la arena.
 * Esta función es el puente entre el sistema de memoria y el manejador de bloques.
 *
 * @return Puntero al primer bloque de la lista enlazada.
 */
void *memory_first_block(void) {
    return blocks_first();
}
