#include <stdio.h>
#include "print.h"
#include "blocks.h"
#include "memory.h"

void mem_print(void) {
    Block *b = memory_first_block();

    size_t total = 0;
    size_t free_total = 0;
    size_t used_total = 0;
    size_t free_blocks = 0;

    printf("\n=== Estado del heap ===\n");

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

    printf("\n--- Resumen ---\n");
    printf("Memoria total:       %zu bytes\n", total);
    printf("Memoria usada:       %zu bytes\n", used_total);
    printf("Memoria libre:       %zu bytes\n", free_total);
    printf("Bloques libres:      %zu\n", free_blocks);
    printf("======================\n\n");
}
