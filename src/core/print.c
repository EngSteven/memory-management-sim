/**
 * @file print.c
 * @brief Rutinas de salida para visualizar el estado actual del heap simulado.
 *
 * Esta unidad provee funciones dedicadas a imprimir la información de la
 * memoria administrada, mostrando cada bloque con su tamaño, desplazamiento
 * y estado (libre u ocupado), así como un resumen general del uso total
 * de la memoria. Se utiliza principalmente para depuración y análisis del
 * comportamiento del simulador de gestión de memoria.
 */

#include <stdio.h>
#include "print.h"
#include "blocks.h"
#include "memory.h"

/**
 * @brief Imprime el estado completo del heap simulado.
 *
 * Recorre la lista enlazada de bloques de memoria administrados y muestra:
 * - El offset (desplazamiento) del bloque dentro del heap.
 * - El tamaño del bloque en bytes.
 * - Si el bloque está marcado como libre (FREE) u ocupado (USED).
 *
 * Además, genera un resumen global del heap, indicando:
 * - Memoria total administrada.
 * - Memoria actualmente usada.
 * - Memoria libre disponible.
 * - Cantidad total de bloques libres.
 *
 * Esta función se utiliza típicamente después de operaciones ALLOC, FREE,
 * REALLOC o en respuesta al comando PRINT del simulador.
 *
 * @note No modifica el estado interno del heap; únicamente realiza operaciones
 *       de lectura e impresión.
 */
void mem_print(void) {
    Block *b = memory_first_block();

    size_t total = 0;
    size_t free_total = 0;
    size_t used_total = 0;
    size_t free_blocks = 0;

    printf("\n=== Estado del heap ===\n");

    /* Recorremos todos los bloques administrados en la lista */
    while (b) {
        printf("  [offset=%zu size=%zu %s]\n",
               b->offset,
               b->size,
               b->is_free ? "FREE" : "USED");

        total += b->size;

        if (b->is_free) {
            free_total += b->size;
            free_blocks++;
        } else {
            used_total += b->size;
        }

        b = b->next;
    }

    /* Resumen general del estado de memoria */
    printf("\n--- Resumen ---\n");
    printf("Memoria total:       %zu bytes\n", total);
    printf("Memoria usada:       %zu bytes\n", used_total);
    printf("Memoria libre:       %zu bytes\n", free_total);
    printf("Bloques libres:      %zu\n", free_blocks);
    printf("======================\n\n");
}
