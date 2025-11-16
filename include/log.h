#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/*
 * Utilidades de logging sencillas.
 */

void log_info(const char *fmt, ...);
void log_error(const char *fmt, ...);

#endif /* LOG_H */
