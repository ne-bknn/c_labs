#include <stdbool.h>

#ifndef LIB_H
#define LIB_H
void print_warning(char msg[]);
void print_error(char msg[]);
int get_int(int* a);

struct Tuple {
	int a, b;
};

struct Tuple count_and_multiply(int n);
bool tuples_eq(struct Tuple* a, struct Tuple* b);
#endif
