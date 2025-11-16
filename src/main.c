#include <stdio.h>
#include "memory.h"
#include "variables.h"
#include "parser.h"
#include "allocator.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo_de_comandos>\n", argv[0]);
        return 1;
    }

    memory_init(2000);   // puedes ajustar el tamaño según lo que necesites
    vars_init();

    //allocator_set_algorithm(ALLOC_FIRST_FIT);
    //allocator_set_algorithm(ALLOC_BEST_FIT);
    //allocator_set_algorithm(ALLOC_WORST_FIT);

    parser_execute_file(argv[1]);

    printf("\n=== Revisión de fugas ===\n");
    var_print_leaks();

    vars_destroy();
    memory_destroy();
    return 0;
}
