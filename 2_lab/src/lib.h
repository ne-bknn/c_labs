#ifndef LIB_H
#define LIB_H

#include <stdio.h>

// Output utils
#define GREEN_BKGR "\e[42m"
#define BLACK_TEXT "\e[0;30m"

#define DEBUG 1
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

void clear_screen();
void print_error(char msg[]);
void print_active(char *msg);
#endif
