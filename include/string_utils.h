/**
 * @file string_utils.h
 * @brief Funciones utilitarias para manipulación de cadenas.
 *
 * Este módulo proporciona operaciones comunes sobre strings, incluyendo
 * recorte de espacios y verificación de prefijos. Las funciones están
 * diseñadas para facilitar el manejo de cadenas en el simulador de memoria
 * y otros componentes del sistema.
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

/**
 * @brief Elimina espacios en blanco al inicio y al final de una cadena.
 *
 * Esta función modifica la cadena original para remover espacios,
 * tabulaciones y saltos de línea en ambos extremos. No realiza copias;
 * retorna un puntero dentro del mismo buffer recibido.
 *
 * @param s Cadena de entrada que será modificada.
 * @return char* Puntero a la cadena recortada. Si `s` es NULL, retorna NULL.
 */
char *str_trim(char *s);

/**
 * @brief Verifica si una cadena comienza con un prefijo dado.
 *
 * Compara carácter por carácter para determinar si `s` inicia exactamente
 * con el contenido de `prefix`. No se realizan operaciones destructivas.
 *
 * @param s Cadena que se desea evaluar.
 * @param prefix Prefijo a comparar.
 * @return int Retorna 1 si la cadena inicia con el prefijo, 0 en caso contrario.
 */
int str_starts_with(const char *s, const char *prefix);

#endif /* STRING_UTILS_H */
