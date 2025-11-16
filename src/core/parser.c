/**
 * @file parser.c
 * @brief Módulo encargado de la lectura, interpretación y ejecución de comandos
 *        provenientes del archivo de entrada para la simulación de memoria.
 *
 * Este parser lee un archivo línea por línea, elimina espacios, ignora comentarios
 * o líneas vacías, interpreta los comandos ALLOC, REALLOC, FREE y PRINT, y despacha
 * las operaciones correspondientes hacia los módulos de memoria.
 *
 * Formato esperado del archivo:
 *   - ALLOC <nombre> <tamaño>
 *   - REALLOC <nombre> <nuevo_tamaño>
 *   - FREE <nombre>
 *   - PRINT
 *   - # comentarios
 */

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

/**
 * @brief Ejecuta todas las instrucciones almacenadas en un archivo.
 *
 * Esta función abre el archivo especificado, procesa cada línea removiendo
 * espacios en blanco y comentarios, valida el formato de los comandos e invoca
 * la operación correspondiente según el tipo de instrucción leída.
 *
 * Comandos soportados:
 *   - **ALLOC nombre tamaño**: Reserva memoria para una variable.
 *   - **REALLOC nombre tamaño**: Cambia el tamaño de un bloque existente.
 *   - **FREE nombre**: Libera un bloque previamente asignado.
 *   - **PRINT**: Muestra el estado actual de la memoria gestionada.
 *
 * Manejo de errores:
 *   - Archivo inexistente.
 *   - Líneas con formato incorrecto.
 *   - Comandos desconocidos.
 *
 * @param path Ruta del archivo que contiene los comandos a procesar.
 * @return 0 si se ejecutó correctamente, -1 si ocurrió un error al abrir el archivo.
 */
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

        // Mostrar la línea actual para depuración
        printf(">> %s\n", line);

        // Buffers para tokens
        char cmd[32];
        char arg1[64];
        size_t size = 0;

        // Patrones esperados:
        // ALLOC X 100
        // REALLOC X 300
        // FREE X
        // PRINT
        int count = sscanf(line, "%31s %63s %zu", cmd, arg1, &size);

        if (count <= 0) {
            log_error("Línea %d: comando inválido", line_number);
            continue;
        }

        // Convertir comando a mayúsculas para manejo uniforme
        for (int i = 0; cmd[i]; i++)
            cmd[i] = toupper((unsigned char)cmd[i]);

        // --- PRINT ---
        if (strcmp(cmd, "PRINT") == 0) {
            mem_print();
            continue;
        }

        // --- FREE nombre ---
        if (strcmp(cmd, "FREE") == 0) {
            if (count < 2) {
                log_error("Línea %d: FREE requiere un nombre", line_number);
                continue;
            }
            mem_free(arg1);
            continue;
        }

        // --- ALLOC nombre tamaño ---
        if (strcmp(cmd, "ALLOC") == 0) {
            if (count < 3) {
                log_error("Línea %d: ALLOC requiere nombre y tamaño", line_number);
                continue;
            }
            mem_alloc(arg1, size);
            continue;
        }

        // --- REALLOC nombre tamaño ---
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
