#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef NE_BKNN_UTILS_H
#define NE_BKNN_UTILS_H

#ifndef ANSI_COLORS
#define ANSI_COLORS
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#define print_debug(fmt, ...) \
	do { if (DEBUG) { fprintf(stderr, ANSI_COLOR_CYAN"%s", "[.] "); fprintf(stderr, fmt, __VA_ARGS__); fprintf(stderr, "%s", ANSI_COLOR_RESET "\n"); } } while (0)

#define free_z(p)                \
	do {                     \
		if (p) {         \
			free(p); \
			(p) = 0; \
                }                \
        } while (0)

#define mknew(t) ((t*)calloc(1, sizeof(t)))

static inline char* strnew(char *original) {
	char* copy = (char*)malloc((strlen(original)+1)*sizeof(char));
	strcpy(copy, original);
	return copy;
}

#endif
