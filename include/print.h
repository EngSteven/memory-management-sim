/**
 * @file print.h
 * @brief Declaraciones relacionadas con la visualización del estado de la memoria.
 *
 * Este módulo contiene las funciones necesarias para imprimir el estado
 * actual del simulador de memoria. Su objetivo es proporcionar una
 * representación legible de las asignaciones, bloques libres, fragmentación
 * y cualquier otra información relevante del administrador de memoria.
 */

#ifndef PRINT_H
#define PRINT_H

/**
 * @brief Imprime el estado actual de la memoria simulada.
 *
 * La función muestra una representación clara de todos los bloques
 * asignados y libres dentro del espacio de memoria administrado.
 * 
 * Información comúnmente incluida:
 *  - Direcciones base de cada bloque
 *  - Tamaño de los bloques usados y libres
 *  - Variables asociadas a cada bloque asignado
 *  - Fragmentación presente en la memoria
 *
 * Esta función es utilizada principalmente para depuración y análisis
 * del comportamiento de los algoritmos de asignación (first-fit, best-fit,
 * worst-fit).
 */
void mem_print(void);

#endif /* PRINT_H */
