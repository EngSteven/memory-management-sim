/**
 * @file log.h
 * @brief Utilidades simples para manejo de mensajes de logging.
 *
 * Este módulo provee funciones básicas para imprimir mensajes de información
 * y de error en la salida estándar. Está diseñado para ser ligero y usable
 * en cualquier parte del simulador de memoria sin acoplamiento adicional.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/**
 * @brief Imprime un mensaje informativo con formato.
 *
 * Función envoltorio de `printf` para mostrar mensajes de información.
 * Acepta una cadena de formato y un número variable de argumentos, igual
 * que `printf`.
 *
 * @param fmt Cadena de formato en estilo printf.
 * @param ... Lista variable de argumentos correspondientes al formato.
 *
 * @note El mensaje se imprime en stdout.
 */
void log_info(const char *fmt, ...);

/**
 * @brief Imprime un mensaje de error con formato.
 *
 * Similar a `log_info`, pero orientado a mostrar mensajes de error.
 * Los mensajes se imprimen en stderr para diferenciarlos de la salida normal.
 *
 * @param fmt Cadena de formato en estilo printf.
 * @param ... Lista variable de argumentos correspondientes al formato.
 *
 * @note El mensaje se imprime en stderr.
 */
void log_error(const char *fmt, ...);

#endif /* LOG_H */
