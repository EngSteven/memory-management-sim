#ifndef MEMORY_OPS_H
#define MEMORY_OPS_H

#include <stddef.h>

int mem_alloc(const char *name, size_t size);
int mem_free(const char *name);
int mem_realloc(const char *name, size_t new_size); 

#endif /* MEMORY_OPS_H */
