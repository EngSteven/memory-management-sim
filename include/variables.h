/**
 * @file variables.h
 * @brief Gestión de la tabla de variables usadas en la simulación de memoria.
 *
 * Este módulo implementa una tabla de símbolos simple (name → Block*), utilizada
 * para mapear nombres de variables definidos en el archivo de entrada a los
 * bloques de memoria asignados dentro del simulador.  
 * 
 * La tabla permite registrar, recuperar, actualizar y eliminar variables, así como
 * detectar fugas de memoria al finalizar la ejecución.
 */

#ifndef VARIABLES_H
#define VARIABLES_H

#include "blocks.h"

/**
 * @struct VarEntry
 * @brief Nodo de una lista enlazada que representa una variable del programa.
 *
 * Cada entrada asocia:
 * - un nombre de variable (`name`)
 * - un puntero a un bloque de memoria asignado (`block`)
 *
 * La tabla completa se implementa como una lista enlazada simple.
 */
typedef struct VarEntry {
    char            *name;   /**< Nombre simbólico de la variable (ALOC, REALLOC). */
    Block           *block;  /**< Puntero al bloque de memoria asociado. */
    struct VarEntry *next;   /**< Siguiente nodo en la lista. */
} VarEntry;

/**
 * @brief Inicializa la tabla de variables.
 *
 * Debe llamarse antes de cualquier operación de asignación o búsqueda de variables.
 */
void vars_init(void);

/**
 * @brief Libera toda la tabla de variables.
 *
 * No libera los bloques de memoria asociados (eso lo debe hacer el simulador).
 * Esta función se utiliza al finalizar el programa.
 */
void vars_destroy(void);

/**
 * @brief Registra o actualiza una variable en la tabla.
 *
 * Si la variable no existe, se crea una nueva entrada.
 * Si ya existe, se actualiza el puntero al bloque.
 *
 * @param name Nombre de la variable.
 * @param block Puntero al bloque asignado.
 */
void var_set(const char *name, Block *block);

/**
 * @brief Obtiene el bloque asociado a una variable.
 *
 * @param name Nombre de la variable.
 * @return Block* Puntero al bloque, o NULL si la variable no existe.
 */
Block *var_get(const char *name);

/**
 * @brief Elimina una variable de la tabla.
 *
 * Solo elimina la entrada de la tabla; no libera la memoria del bloque.
 *
 * @param name Nombre de la variable a eliminar.
 */
void var_remove(const char *name);

/**
 * @brief Imprime variables que aún tienen bloques asignados.
 *
 * Se utiliza para detectar fugas de memoria al final de la ejecución.
 */
void var_print_leaks(void);

#endif /* VARIABLES_H */
