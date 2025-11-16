/**
 * @file blocks.c
 * @brief Implementación de la lista doblemente enlazada de bloques de memoria
 *        utilizada por el simulador para administrar fragmentos libres y ocupados.
 *
 * Este módulo gestiona operaciones clave sobre bloques de memoria:
 * - Creación de bloques.
 * - Fragmentación (split) de bloques grandes.
 * - Fusión (merge) de bloques libres contiguos.
 * - Acceso secuencial a la lista doblemente enlazada.
 * - Liberación completa de la estructura al finalizar la ejecución.
 *
 * La estructura mantiene un puntero global al primer bloque, actuando como la
 * cabecera de la lista sobre la cual operan los algoritmos de asignación.
 */

#include <stdlib.h>
#include "blocks.h"
#include "log.h"

/** 
 * @brief Puntero al primer bloque de la lista doblemente enlazada.
 *
 * Representa el estado inicial de la memoria simulada.
 */
static Block *first_block = NULL;

/**
 * @brief Crea un nuevo bloque de memoria en la lista.
 *
 * Inicializa un bloque con su offset, tamaño y estado (libre/ocupado).
 * No realiza inserciones excepto cuando la lista está vacía.
 *
 * @param offset  Desplazamiento del bloque dentro del arreglo de memoria simulado.
 * @param size    Tamaño en bytes del bloque.
 * @param is_free Indica si el bloque está libre (true) u ocupado (false).
 *
 * @return Puntero al bloque recién creado, o NULL si malloc falla.
 */
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

    /* Si no hay bloques previos, este se convierte en el primero */
    if (first_block == NULL) {
        first_block = b;
    }

    return b;
}

/**
 * @brief Obtiene el primer bloque de la lista.
 *
 * @return Puntero al primer bloque o NULL si la lista está vacía.
 */
Block *blocks_first(void) {
    return first_block;
}

/**
 * @brief Obtiene el siguiente bloque en la lista.
 *
 * @param block Bloque actual.
 * @return Puntero al siguiente bloque o NULL si no existe.
 */
Block *blocks_next(Block *block) {
    if (!block) return NULL;
    return block->next;
}

/**
 * @brief Divide un bloque en dos cuando el tamaño solicitado es menor al bloque original.
 *
 * La primera parte mantiene el tamaño solicitado.  
 * La segunda parte se crea como un nuevo bloque libre.
 *
 * @param block Bloque a dividir.
 * @param size  Tamaño solicitado para el nuevo bloque reducido.
 */
void block_split(Block *block, size_t size) {
    /* No dividir si no hay espacio suficiente o si coincide exactamente */
    if (block->size <= size) {
        return;
    }

    /* Crear bloque restante */
    Block *rest = malloc(sizeof(Block));
    if (!rest) {
        log_error("Error: malloc falló en block_split()");
        return;
    }

    rest->offset  = block->offset + size;
    rest->size    = block->size - size;
    rest->is_free = true;

    /* Enlazar resto en la lista */
    rest->next = block->next;
    rest->prev = block;

    if (block->next) {
        block->next->prev = rest;
    }

    block->next = rest;

    /* Ajustar tamaño del bloque original */
    block->size = size;
}

/**
 * @brief Fusiona un bloque libre con sus vecinos libres.
 *
 * El merge se realiza primero con el bloque anterior, luego con el siguiente.
 * La función devuelve el bloque resultante después de cualquier combinación.
 *
 * @param block Bloque que se intentará fusionar.
 * @return Puntero al bloque resultante tras el merge (puede ser el original o uno previo).
 */
Block *block_merge(Block *block) {
    if (!block) return NULL;

    Block *b = block;

    /* Intento de merge con el bloque anterior */
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

    /* Intento de merge con el siguiente */
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

/**
 * @brief Libera toda la lista de bloques.
 *
 * Se utiliza al finalizar la ejecución del programa para limpiar toda la memoria
 * asociada a la estructura de administración.
 */
void blocks_destroy(void) {
    Block *curr = first_block;
    while (curr) {
        Block *next = curr->next;
        free(curr);
        curr = next;
    }
    first_block = NULL;
}
