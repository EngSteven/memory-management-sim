/**
 * @file parser.h
 * @brief Interfaz del módulo de análisis y ejecución de comandos de memoria.
 *
 * Este módulo se encarga de leer un archivo de entrada que contiene
 * instrucciones de gestión de memoria (ALLOC, FREE, REALLOC, PRINT)
 * y ejecutar cada operación en el orden especificado.
 *
 * Las líneas que comienzan con '#' se consideran comentarios y se omiten.
 */

#ifndef PARSER_H
#define PARSER_H

/**
 * @brief Ejecuta un archivo de comandos de memoria.
 *
 * Esta función abre el archivo ubicado en `path`, lee línea por línea
 * las instrucciones válidas (ALLOC, FREE, REALLOC, PRINT) e invoca
 * las funciones correspondientes del simulador de memoria.
 *
 * Maneja comentarios, errores sintácticos y reporte básico
 * en caso de líneas inválidas.
 *
 * @param path Ruta del archivo a procesar.
 *
 * @return 0 si el archivo se procesó correctamente,
 *         un valor distinto de cero en caso de error
 *         (archivo inexistente, error de lectura, etc.).
 */
int parser_execute_file(const char *path);

#endif /* PARSER_H */
