#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "string_utils.h"
#include "memory_ops.h"
#include "print.h"
#include "log.h"

#define MAX_LINE 256

int parser_execute_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        log_error("No se pudo abrir el archivo '%s'", path);
        return -1;
    }

    char buffer[MAX_LINE];
    int line_number = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        line_number++;

        // Remover espacios en inicio y final
        char *line = str_trim(buffer);

        // Ignorar líneas vacías
        if (line[0] == '\0') continue;

        // Ignorar comentarios
        if (line[0] == '#') continue;

        // Imprimir la línea que se está ejecutando
        printf(">> %s\n", line);

        // Parsear comando y argumentos
        char cmd[32];
        char arg1[64];
        size_t size = 0;

        // Casos:
        // ALLOC X 100
        // REALLOC X 300
        // FREE X
        // PRINT
        int count = sscanf(line, "%31s %63s %zu", cmd, arg1, &size);

        if (count <= 0) {
            log_error("Línea %d: comando inválido", line_number);
            continue;
        }

        // Normalizar comando a mayúsculas
        for (int i = 0; cmd[i]; i++)
            cmd[i] = toupper((unsigned char)cmd[i]);

        // --- PRINT ---
        if (strcmp(cmd, "PRINT") == 0) {
            mem_print();
            continue;
        }

        // --- FREE name ---
        if (strcmp(cmd, "FREE") == 0) {
            if (count < 2) {
                log_error("Línea %d: FREE requiere un nombre", line_number);
                continue;
            }
            mem_free(arg1);
            continue;
        }

        // --- ALLOC name size ---
        if (strcmp(cmd, "ALLOC") == 0) {
            if (count < 3) {
                log_error("Línea %d: ALLOC requiere nombre y tamaño", line_number);
                continue;
            }
            mem_alloc(arg1, size);
            continue;
        }

        // --- REALLOC name size ---
        if (strcmp(cmd, "REALLOC") == 0) {
            if (count < 3) {
                log_error("Línea %d: REALLOC requiere nombre y tamaño", line_number);
                continue;
            }
            mem_realloc(arg1, size);
            continue;
        }

        // Comando desconocido
        log_error("Línea %d: comando '%s' no reconocido", line_number, cmd);
    }

    fclose(f);
    return 0;
}
