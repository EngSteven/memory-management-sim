/**
 * @file blocks.h
 * @brief Estructuras y operaciones para la gestión interna de bloques de memoria.
 *
 * Este módulo define la estructura Block, utilizada para representar segmentos
 * dentro de la arena de memoria simulada, así como las funciones auxiliares
 * para dividir, unir y recorrer dichos bloques.  
 *
 * Forma parte del sistema de administración de memoria del proyecto, donde
 * cada bloque representa una porción contigua de memoria, marcada como libre
 * u ocupada, y encadenada mediante una lista doblemente enlazada.
 */

#ifndef BLOCKS_H
#define BLOCKS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @struct Block
 * @brief Representa un bloque dentro de la arena de memoria.
 *
 * Cada bloque mantiene su posición relativa dentro de la arena (offset),
 * su tamaño, un indicador de si está libre y punteros al bloque anterior
 * y siguiente en la lista doblemente enlazada.
 */
typedef struct Block {
    size_t offset;      /**< Desplazamiento inicial dentro de la arena. */
    size_t size;        /**< Tamaño del bloque en bytes. */
    bool   is_free;     /**< Indica si el bloque está libre (true) u ocupado (false). */
    struct Block *next; /**< Puntero al siguiente bloque en la lista. */
    struct Block *prev; /**< Puntero al bloque anterior en la lista. */
} Block;

/**
 * @brief Crea un nuevo bloque de memoria.
 *
 * Inicializa una estructura Block con el offset, tamaño y estado indicado.
 *
 * @param offset Posición inicial del bloque dentro de la arena.
 * @param size   Tamaño del bloque en bytes.
 * @param is_free Estado del bloque (true = libre, false = ocupado).
 * @return Puntero al nuevo bloque creado, o NULL si falla la asignación.
 */
Block *block_create(size_t offset, size_t size, bool is_free);

/**
 * @brief Divide un bloque en dos.
 *
 * Reduce el tamaño del bloque original al valor indicado y crea un nuevo bloque
 * contiguo con el tamaño restante. El nuevo bloque queda marcado como libre.
 *
 * @param block Bloque a dividir.
 * @param size  Tamaño que debe conservar el bloque original.
 */
void block_split(Block *block, size_t size);

/**
 * @brief Intenta fusionar un bloque con sus vecinos libres.
 *
 * Si el bloque anterior o siguiente están libres, los combina en un único bloque
 * contiguo para reducir la fragmentación externa.
 *
 * @param block Bloque que se intentará fusionar.
 * @return Puntero al bloque resultante tras las fusiones.
 */
Block *block_merge(Block *block);

/**
 * @brief Obtiene el primer bloque de la lista.
 *
 * @return Puntero al primer bloque, o NULL si la lista está vacía.
 */
Block *blocks_first(void);

/**
 * @brief Obtiene el siguiente bloque en la lista.
 *
 * @param block Bloque actual.
 * @return Puntero al bloque siguiente, o NULL si no hay más.
 */
Block *blocks_next(Block *block);

/**
 * @brief Libera todos los bloques y destruye la lista completa.
 *
 * Utilizada al finalizar la ejecución del programa o al reiniciar la arena.
 */
void blocks_destroy(void);

#endif /* BLOCKS_H */
