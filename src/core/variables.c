#include <stdlib.h>
#include <string.h>
#include "variables.h"
#include "log.h"

/*
 * Lista global de variables registradas.
 */
static VarEntry *var_list = NULL;

/* Implementación propia de strdup (C11-compatible) */
static char *var_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len + 1); // copia también el '\0'
    return copy;
}

void vars_init(void) {
    var_list = NULL;
}

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

static VarEntry *var_find(const char *name) {
    VarEntry *curr = var_list;
    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void var_set(const char *name, Block *block) {
    if (!name || !block) {
        log_error("var_set: argumentos inválidos");
        return;
    }

    VarEntry *entry = var_find(name);

    if (entry) {
        // Ya existe: actualizar bloque
        entry->block = block;
        return;
    }

    // Crear variable nueva
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

Block *var_get(const char *name) {
    VarEntry *entry = var_find(name);
    return entry ? entry->block : NULL;
}

void var_remove(const char *name) {
    VarEntry *curr = var_list;
    VarEntry *prev = NULL;

    while (curr) {
        if (strcmp(curr->name, name) == 0) {

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
