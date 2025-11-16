/**
 * @file string_utils.c
 * @brief Implementación de funciones utilitarias para manejo básico de cadenas.
 *
 * Este módulo proporciona funciones comunes para manipulación de strings utilizadas
 * a lo largo del simulador de memoria. Incluye operaciones de recorte (trim)
 * y validación de prefijos.
 */

#include <ctype.h>
#include <string.h>
#include "string_utils.h"

/**
 * @brief Elimina espacios en blanco al inicio y final de una cadena.
 *
 * Esta función modifica la cadena original eliminando todos los caracteres
 * considerados espacio en blanco (según `isspace`) en ambos extremos.
 *
 * El recorte se realiza in-place y garantiza que el resultado termine con
 * '\0'. Si la cadena es NULL, simplemente devuelve NULL.
 *
 * @param s Cadena a recortar. Debe ser modificable.
 * @return char* Puntero a la cadena ya recortada. Puede ser distinto al original
 *         si había espacios al inicio.
 */
char *str_trim(char *s) {
    if (!s) return s;

    // Recorte inicial
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }

    // Recorte final
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';

    return s;
}

/**
 * @brief Verifica si una cadena comienza con un prefijo dado.
 *
 * Compara carácter por carácter para determinar si `s` inicia con `prefix`.
 * Ambas cadenas deben ser válidas (no NULL). La comparación es exacta y
 * sensible a mayúsculas/minúsculas.
 *
 * @param s Cadena principal.
 * @param prefix Prefijo a verificar.
 * @return int Devuelve 1 si la cadena inicia con el prefijo, 0 en caso contrario.
 */
int str_starts_with(const char *s, const char *prefix) {
    if (!s || !prefix) return 0;

    while (*prefix) {
        if (*s++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}
