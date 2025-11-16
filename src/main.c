/**
 * @file main.c
 * @brief Punto de entrada del simulador de gestión de memoria.
 *
 * Este programa ejecuta una simulación de administración dinámica de memoria
 * basada en un archivo de comandos que contiene operaciones como ALLOC, FREE,
 * REALLOC y PRINT. El sistema utiliza un bloque de memoria propio y permite
 * seleccionar entre algoritmos de asignación (First-Fit, Best-Fit, Worst-Fit).
 *
 * El flujo principal incluye:
 *  - Inicialización del bloque de memoria simulado.
 *  - Inicialización del sistema de variables asignadas.
 *  - Ejecución del parser de comandos desde el archivo de entrada.
 *  - Impresión de fugas de memoria detectadas.
 *  - Liberación de recursos utilizados.
 */

#include <stdio.h>
#include "memory.h"
#include "variables.h"
#include "parser.h"
#include "allocator.h"

/**
 * @brief Función principal del simulador.
 *
 * Controla el ciclo de vida completo de la simulación: inicializa los
 * subsistemas, procesa el archivo de comandos y finalmente verifica fugas de
 * memoria y destruye estructuras internas.
 *
 * @param argc Cantidad de argumentos pasados al programa.
 * @param argv Lista de argumentos; se espera argv[1] como archivo de comandos.
 * @return 0 si la ejecución fue exitosa, 1 si hubo errores en los argumentos.
 *
 * **Uso esperado:**
 * ```
 * ./memsim comandos.txt
 * ```
 */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo_de_comandos>\n", argv[0]);
        return 1;
    }

    // Inicialización del bloque de memoria simulado (tamaño ajustable)
    memory_init(2000);

    // Inicialización del sistema de variables manejadas por nombre
    vars_init();

    // Desmarcar el algoritmo de asignación deseado o usar el First-Fit por defecto
    
    // allocator_set_algorithm(ALLOC_FIRST_FIT);
    // allocator_set_algorithm(ALLOC_BEST_FIT);
    // allocator_set_algorithm(ALLOC_WORST_FIT);

    // Procesa el archivo de comandos indicado por el usuario
    parser_execute_file(argv[1]);

    printf("\n=== Revisión de fugas ===\n");
    var_print_leaks();

    // Limpieza de estructuras internas
    vars_destroy();
    memory_destroy();

    return 0;
}
