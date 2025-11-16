#ifndef VARIABLES_H
#define VARIABLES_H

#include "blocks.h"

/*
 * Tabla de variables: nombre -> bloque.
 */

typedef struct VarEntry {
    char        *name;
    Block       *block;
    struct VarEntry *next;
} VarEntry;

void      vars_init(void);
void      vars_destroy(void);
void      var_set(const char *name, Block *block);
Block    *var_get(const char *name);
void      var_remove(const char *name);
void      var_print_leaks(void);

#endif /* VARIABLES_H */
