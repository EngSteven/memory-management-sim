#ifndef PARSER_H
#define PARSER_H

/*
 * Parser de archivos de comandos (ALLOC, FREE, REALLOC, PRINT).
 */

int parser_execute_file(const char *path);

#endif /* PARSER_H */
