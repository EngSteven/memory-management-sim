/**
 * @file memory_ops.c
 * @brief Implementación de las operaciones de memoria ALLOC, FREE y REALLOC
 *        para el simulador de administración de memoria.
 *
 * Este módulo proporciona las funciones principales que manipulan bloques de la arena,
 * registran variables, aplican los algoritmos de asignación (first-fit, best-fit, worst-fit),
 * realizan splits/merges y garantizan consistencia interna.
 */

#include <string.h>
#include <stdlib.h>
#include "memory_ops.h"
#include "allocator.h"
#include "blocks.h"
#include "variables.h"
#include "memory.h"
#include "log.h"

/**
 * @brief Asigna memoria simulada (equivalente a ALLOC).
 *
 * Realiza:
 *  - Validación de nombre duplicado
 *  - Selección de bloque según algoritmo configurado
 *  - Split del bloque si es mayor al solicitado
 *  - Registro de la variable
 *  - Relleno de la arena con la primera letra del nombre
 *
 * @param name Nombre simbólico de la variable.
 * @param size Cantidad de bytes solicitados.
 * @return 0 si la operación fue exitosa, -1 si ocurrió algún error.
 */
int mem_alloc(const char *name, size_t size) {

    /* 1. Validar duplicado */
    if (var_get(name) != NULL) {
        log_error("ALLOC: variable '%s' ya existe", name);
        return -1;
    }

    /* 2. Buscar bloque libre según first/best/worst fit */
    Block *block = allocator_find_block(size);
    if (!block) {
        log_error("ALLOC: no hay bloque libre suficiente para '%s' (%zu bytes)", name, size);
        return -1;
    }

    /* 3. Split si el bloque es más grande */
    if (block->size > size) {
        block_split(block, size);
    }

    /* 4. Marcar bloque como ocupado */
    block->is_free = false;

    /* 5. Registrar variable */
    var_set(name, block);

    /* 6. Rellenar la arena */
    unsigned char *arena = (unsigned char *)memory_arena();
    for (size_t i = 0; i < size; i++) {
        arena[block->offset + i] = name[0]; /* Se usa solo la primera letra del nombre */
    }

    log_info("ALLOC '%s' (%zu bytes) en offset=%zu", name, size, block->offset);
    return 0;
}

/**
 * @brief Libera memoria asociada a una variable (equivalente a FREE).
 *
 * Realiza:
 *  - Obtención del bloque asociado
 *  - Marcado como libre
 *  - Fusión (merge) con vecinos libres
 *  - Eliminación de la variable de la tabla
 *
 * @param name Nombre de la variable a liberar.
 * @return 0 si fue liberada exitosamente, -1 si la variable no existe.
 */
int mem_free(const char *name) {

    /* 1. Obtener bloque asociado */
    Block *b = var_get(name);
    if (!b) {
        log_error("FREE: variable '%s' no existe", name);
        return -1;
    }

    /* 2. Marcar bloque como libre */
    b->is_free = true;

    /* 3. Merge con vecinos libres */
    block_merge(b);

    /* 4. Eliminar variable de la tabla */
    var_remove(name);

    log_info("FREE '%s'", name);
    return 0;
}

/**
 * @brief Cambia el tamaño de un bloque de memoria (REALLOC).
 *
 * Casos manejados:
 *  - new_size == 0 → equivalente a FREE
 *  - new_size == old_size → no hace nada
 *  - Reducción del tamaño con posible split
 *  - Expansión in-place si hay espacio libre contiguo
 *  - Movimiento a un nuevo bloque si no es posible expandir
 *
 * @param name Nombre de la variable existente.
 * @param new_size Nuevo tamaño solicitado en bytes.
 * @return 0 si la operación fue exitosa, -1 si falló o no hay espacio.
 */
int mem_realloc(const char *name, size_t new_size) {

    Block *old = var_get(name);
    if (!old) {
        log_error("REALLOC: variable '%s' no existe", name);
        return -1;
    }

    size_t old_size = old->size;

    /* Caso 0: new_size == 0 → liberar memoria */
    if (new_size == 0) {
        return mem_free(name);
    }

    /* Caso 1: mismo tamaño → no se hace nada */
    if (new_size == old_size) {
        return 0;
    }

    unsigned char *arena = memory_arena();

    /* Caso 2: reducción */
    if (new_size < old_size) {
        block_split(old, new_size);   /* Ajusta el bloque actual */
        block_merge(old);             /* Intenta fusionar sobrante */
        log_info("REALLOC (reduce) '%s' %zu -> %zu bytes", name, old_size, new_size);
        return 0;
    }

    /* Caso 3: expansión in-place */
    size_t extra = new_size - old_size;

    /* Intentar coalescer espacio contiguo */
    if (old->next && old->next->is_free) {
        block_merge(old->next);
    }

    /* Revisar si ya hay suficiente espacio */
    if (old->next && old->next->is_free && old->next->size >= extra) {

        Block *next = old->next;

        if (next->size == extra) {
            /* Tomar todo el bloque next */
            old->size += next->size;

            old->next = next->next;
            if (next->next) {
                next->next->prev = old;
            }
            free(next);
        } else {
            /* Consumir parte del bloque siguiente */
            next->offset += extra;
            next->size   -= extra;
            old->size    += extra;
        }

        /* Rellenar la parte nueva */
        for (size_t i = old_size; i < new_size; i++) {
            arena[old->offset + i] = name[0];
        }

        log_info("REALLOC (expand in-place) '%s' %zu -> %zu bytes", name, old_size, new_size);
        return 0;
    }

    /* Caso 4: mover a un nuevo bloque */
    Block *new_block = allocator_find_block(new_size);
    if (!new_block) {
        log_error("REALLOC: no hay bloque nuevo suficiente para '%s'", name);
        return -1;
    }

    /* Split si es mayor */
    if (new_block->size > new_size) {
        block_split(new_block, new_size);
    }

    new_block->is_free = false;

    /* Copiar contenido */
    memcpy(arena + new_block->offset,
           arena + old->offset,
           old_size);

    /* Rellenar el resto */
    for (size_t i = old_size; i < new_size; i++) {
        arena[new_block->offset + i] = name[0];
    }

    /* Liberar bloque original */
    mem_free(name);

    /* Registrar nuevo bloque */
    var_set(name, new_block);

    log_info("REALLOC (move) '%s' %zu -> %zu bytes", name, old_size, new_size);
    return 0;
}
