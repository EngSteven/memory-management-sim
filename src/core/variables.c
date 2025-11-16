/**
 * @file variables.c
 * @brief Gestión de variables simbólicas asociadas a bloques de memoria.
 *
 * Este módulo implementa una tabla de símbolos simple basada en una lista enlazada.
 * Permite registrar, obtener, actualizar y eliminar variables que apuntan a bloques
 * de memoria simulados dentro del sistema de administración de memoria.
 *
 * También facilita la detección de fugas de memoria reportando variables que nunca
 * fueron liberadas mediante FREE.
 */

#include <stdlib.h>
#include <string.h>
#include "variables.h"
#include "log.h"

/**
 * @brief Lista enlazada global de variables registradas.
 *
 * Cada entrada almacena:
 * - nombre de la variable
 * - bloque de memoria asociado
 * - siguiente nodo
 */
static VarEntry *var_list = NULL;

/**
 * @brief Implementación local de strdup (compatible con C11).
 *
 * Crea una copia dinámica de la cadena recibida. Se implementa manualmente ya
 * que strdup no está garantizada por C11.
 *
 * @param s Cadena original.
 * @return Copia dinámica terminada en '\0', o NULL si falla la asignación.
 */
static char *var_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len + 1); // copia incluyendo el '\0'
    return copy;
}

/**
 * @brief Inicializa la tabla de variables.
 *
 * Debe llamarse al iniciar el programa. Reinicia la lista a NULL.
 */
void vars_init(void) {
    var_list = NULL;
}

/**
 * @brief Destruye todas las variables registradas.
 *
 * Libera la lista enlazada, incluyendo cadenas y nodos.
 * No libera los bloques a los que apuntan —eso le corresponde al manejador
 * de memoria. Solo elimina la asociación simbólica.
 */
void vars_destroy(void) {
    VarEntry *curr = var_list;
    while (curr) {
        VarEntry *next = curr->next;
        free(curr->name);
        free(curr);
        curr = next;
    }
    var_list = NULL;
}

/**
 * @brief Busca una variable por nombre en la lista de variables.
 *
 * @param name Nombre de la variable a buscar.
 * @return Puntero a la entrada si existe, NULL de lo contrario.
 */
static VarEntry *var_find(const char *name) {
    VarEntry *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Registra o actualiza una variable simbólica.
 *
 * Si la variable ya existe, simplemente se actualiza su bloque asociado.
 * Si no existe, se crea una nueva entrada y se inserta al inicio de la lista.
 *
 * @param name Nombre de la variable.
 * @param block Bloque asociado a la variable.
 */
void var_set(const char *name, Block *block) {
    if (!name || !block) {
        log_error("var_set: argumentos inválidos");
        return;
    }

    VarEntry *entry = var_find(name);

    if (entry) {
        /* Ya existe: actualizar referencia */
        entry->block = block;
        return;
    }

    /* Crear nueva entrada */
    entry = malloc(sizeof(VarEntry));
    if (!entry) {
        log_error("var_set: malloc falló");
        return;
    }

    entry->name = var_strdup(name);
    if (!entry->name) {
        free(entry);
        log_error("var_set: var_strdup falló");
        return;
    }

    entry->block = block;
    entry->next = var_list;
    var_list = entry;
}

/**
 * @brief Recupera el bloque asociado a una variable.
 *
 * @param name Nombre de la variable.
 * @return Puntero al bloque asignado, o NULL si no existe.
 */
Block *var_get(const char *name) {
    VarEntry *entry = var_find(name);
    return entry ? entry->block : NULL;
}

/**
 * @brief Elimina una variable registrada.
 *
 * No libera el bloque de memoria; únicamente elimina la asociación simbólica.
 *
 * @param name Nombre de la variable a quitar.
 */
void var_remove(const char *name) {
    VarEntry *curr = var_list;
    VarEntry *prev = NULL;

    while (curr) {
        if (strcmp(curr->name, name) == 0) {

            /* Ajustar enlaces según sea nodo inicial o intermedio */
            if (prev == NULL) {
                var_list = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr->name);
            free(curr);
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

/**
 * @brief Imprime todas las variables aún registradas, indicando fugas de memoria.
 *
 * Si quedan variables sin eliminar (FREE), significa que existe una fuga, ya
 * que su bloque sigue asignado.
 */
void var_print_leaks(void) {
    VarEntry *curr = var_list;

    if (!curr) {
        printf("Sin fugas de memoria.\n");
        return;
    }

    printf("Fugas detectadas:\n");
    while (curr) {
        printf("  Variable '%s' sigue asignada (offset=%zu size=%zu)\n",
               curr->name,
               curr->block ? curr->block->offset : 0,
               curr->block ? curr->block->size   : 0);
        curr = curr->next;
    }
}
