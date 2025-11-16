/**
 * @file log.c
 * @brief Implementación de un sistema simple de logging para el simulador de memoria.
 *
 * Este módulo provee funciones mínimas para imprimir mensajes de información
 * y error empleando `stdout` y `stderr` respectivamente. Admite formatos
 * variádicos al estilo `printf()`, permitiendo mensajes detallados durante
 * la ejecución del simulador.
 */

#include <stdarg.h>
#include <stdio.h>
#include "log.h"

/**
 * @brief Imprime un mensaje informativo en stdout.
 *
 * Esta función permite reportar eventos generales del programa,
 * como operaciones exitosas, cambios de estado o pasos de la simulación.
 * El formato funciona igual que `printf()`, utilizando argumentos variádicos.
 *
 * @param fmt Cadena de formato compatible con `printf()`.
 * @param ... Lista variable de argumentos correspondiente al formato.
 */
void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stdout, "[INFO] ");
    vfprintf(stdout, fmt, args);
    fprintf(stdout, "\n");

    va_end(args);
}

/**
 * @brief Imprime un mensaje de error en stderr.
 *
 * Utilizada para reportar fallos, errores lógicos, condiciones inesperadas,
 * manejo de memoria incorrecto, etc. El formato sigue el estilo `printf()`
 * con soporte para argumentos variádicos.
 *
 * @param fmt Cadena de formato compatible con `printf()`.
 * @param ... Lista variable de argumentos correspondiente al formato.
 */
void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
}
