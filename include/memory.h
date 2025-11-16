/**
 * @file memory.h
 * @brief Gestión de la arena de memoria simulada.
 *
 * Este módulo administra un bloque continuo de memoria (arena)
 * sobre el cual se implementan los algoritmos de asignación 
 * (first-fit, best-fit, worst-fit). Provee funciones para inicializar,
 * limpiar y obtener punteros relevantes dentro de la arena.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

/**
 * @brief Inicializa la arena de memoria simulada.
 *
 * Solicita un bloque contiguo de memoria al sistema operativo mediante
 * `malloc`, el cual será utilizado para simular todas las operaciones
 * dinámicas del sistema (ALLOC, REALLOC, FREE).
 *
 * @param size Tamaño total en bytes del bloque a solicitar.
 *
 * @warning Debe llamarse antes de cualquier operación de asignación simulada.
 * @warning Si la memoria ya estaba inicializada, debe destruirse antes de
 *          realizar una nueva inicialización.
 */
void memory_init(size_t size);

/**
 * @brief Libera completamente la arena de memoria simulada.
 *
 * Libera el bloque solicitado en `memory_init()` y deja los punteros
 * internos en un estado seguro (NULL). Todas las referencias previas
 * a bloques dentro de la arena quedan invalidadas.
 */
void memory_destroy(void);

/**
 * @brief Obtiene un puntero al inicio de la arena de memoria.
 *
 * @return Puntero al bloque principal reservado mediante `memory_init()`,
 *         o NULL si la arena no ha sido inicializada.
 */
void *memory_arena(void);

/**
 * @brief Retorna un puntero al inicio del primer bloque de control.
 *
 * Esta función es útil para recorrer la estructura interna de bloques
 * dentro de la arena (estructura de metadatos de asignación).
 *
 * @return Puntero al primer bloque dentro de la arena simulada.
 */
void *memory_first_block(void);

#endif /* MEMORY_H */
