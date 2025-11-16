/**
 * @file allocator.h
 * @brief Declaraciones del módulo encargado de la selección de bloques
 *        según distintas estrategias de asignación de memoria.
 *
 * Este módulo define las estrategias de asignación de memoria utilizadas
 * por el simulador (First-Fit, Best-Fit y Worst-Fit), así como las funciones
 * encargadas de establecer el algoritmo activo y buscar un bloque adecuado
 * para una solicitud de memoria.
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include "blocks.h"

/**
 * @enum AllocAlgorithm
 * @brief Estrategias de asignación de memoria soportadas.
 *
 * Define el método utilizado por el sistema para seleccionar un bloque
 * libre dentro del área gestionada. Cada estrategia tiene implicaciones
 * en rendimiento, fragmentación y comportamiento general del simulador.
 *
 * - **ALLOC_FIRST_FIT:** Selecciona el primer bloque libre lo suficientemente grande.
 * - **ALLOC_BEST_FIT:** Selecciona el bloque libre más pequeño que pueda contener la solicitud.
 * - **ALLOC_WORST_FIT:** Selecciona el bloque libre más grande disponible.
 */
typedef enum {
    ALLOC_FIRST_FIT,  /**< Primer bloque que encaje. */
    ALLOC_BEST_FIT,   /**< Bloque más pequeño que cumpla el tamaño. */
    ALLOC_WORST_FIT   /**< Bloque libre más grande encontrado. */
} AllocAlgorithm;

/**
 * @brief Establece el algoritmo de asignación de memoria que el simulador utilizará.
 *
 * Esta función configura la estrategia que se aplicará durante las
 * futuras operaciones de ALLOC y REALLOC. No afecta bloques ya asignados.
 *
 * @param algo Estrategia de asignación a utilizar (First-Fit, Best-Fit o Worst-Fit).
 */
void allocator_set_algorithm(AllocAlgorithm algo);

/**
 * @brief Busca un bloque libre adecuado según la estrategia de asignación activa.
 *
 * Recorre la lista de bloques libres y selecciona aquel que cumpla con el tamaño
 * solicitado, aplicando la estrategia especificada mediante
 * `allocator_set_algorithm()`.
 *
 * @param size Tamaño solicitado en bytes.
 * @return Puntero a un bloque libre adecuado, o `NULL` si no se encontró ninguno.
 */
Block *allocator_find_block(size_t size);

#endif /* ALLOCATOR_H */
