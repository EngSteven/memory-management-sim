#include <ctype.h>
#include <string.h>
#include "string_utils.h"

/*
 * Implementación básica de utilidades de string.
 * Se podrá mejorar más adelante si es necesario.
 */

char *str_trim(char *s) {
    if (!s) return s;

    // Trim inicio
    while (*s && isspace((unsigned char)*s)) {
        s++;
    }

    // Trim final
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';

    return s;
}

int str_starts_with(const char *s, const char *prefix) {
    if (!s || !prefix) return 0;
    while (*prefix) {
        if (*s++ != *prefix++) {
            return 0;
        }
    }
    return 1;
}
