/**
 * @file allocator.c
 * @brief Implementación de los algoritmos de asignación dinámica de memoria
 *        (First-Fit, Best-Fit y Worst-Fit) para el simulador.
 *
 * Este módulo se encarga de seleccionar bloques libres dentro de la memoria
 * simulada usando diferentes estrategias de búsqueda. Los algoritmos operan
 * sobre la lista enlazada de bloques administrativos definida en `blocks.c`.
 */

#include <stddef.h>
#include "allocator.h"
#include "blocks.h"
#include "log.h"

/* ------------------------------------------------------------------------- */
/*                        VARIABLES ESTÁTICAS INTERNAS                       */
/* ------------------------------------------------------------------------- */

/**
 * @brief Algoritmo de asignación actualmente activo.
 *
 * El valor por defecto es `ALLOC_FIRST_FIT`. Puede modificarse en tiempo
 * de ejecución mediante `allocator_set_algorithm()`.
 */
static AllocAlgorithm current_algo = ALLOC_FIRST_FIT;

/* ------------------------------------------------------------------------- */
/*                      IMPLEMENTACIÓN DE FIRST-FIT                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Busca el primer bloque libre lo suficientemente grande (First-Fit).
 *
 * Recorre la lista de bloques desde el inicio y retorna el primer bloque
 * libre cuyo tamaño sea mayor o igual al solicitado.
 *
 * @param size Tamaño solicitado en bytes.
 * @return Puntero al bloque adecuado, o `NULL` si no se encontró ninguno.
 */
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

/* ------------------------------------------------------------------------- */
/*                      IMPLEMENTACIÓN DE BEST-FIT                           */
/* ------------------------------------------------------------------------- */

/**
 * @brief Busca el bloque libre más pequeño que sea suficientemente grande
 *        (Best-Fit).
 *
 * Recorre todos los bloques libres y selecciona aquel cuyo tamaño sea
 * el mínimo posible pero aún así >= size. Minimiza fragmentación externa.
 *
 * @param size Tamaño solicitado en bytes.
 * @return El bloque más ajustado, o `NULL` si no existe uno adecuado.
 */
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

/* ------------------------------------------------------------------------- */
/*                      IMPLEMENTACIÓN DE WORST-FIT                          */
/* ------------------------------------------------------------------------- */

/**
 * @brief Busca el bloque libre más grande disponible (Worst-Fit).
 *
 * Selecciona el bloque libre con mayor tamaño. Tiende a reducir
 * fragmentación interna generando huecos más grandes.
 *
 * @param size Tamaño solicitado en bytes.
 * @return El bloque más grande que puede usarse, o `NULL` si no existe.
 */
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

/* ------------------------------------------------------------------------- */
/*                           API DE ASIGNACIÓN                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Cambia el algoritmo de asignación utilizado por el simulador.
 *
 * @param algo El nuevo algoritmo a utilizar (FIRST, BEST o WORST-FIT).
 */
void allocator_set_algorithm(AllocAlgorithm algo) {
    current_algo = algo;
    log_info("Algoritmo de asignación cambiado a %d", (int)algo);
}

/**
 * @brief Selecciona un bloque libre usando el algoritmo configurado.
 *
 * Esta función actúa como fachada, delegando a la función correspondiente
 * según el algoritmo activo.
 *
 * @param size Tamaño solicitado en bytes.
 * @return Un bloque adecuado para la asignación, o `NULL` si no se encuentra.
 */
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
