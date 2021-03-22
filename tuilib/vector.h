#include <stddef.h>
#include <stdlib.h>

#ifndef MY_VECTOR_H
#define MY_VECTOR_H

struct Vector {
	int* data;
	size_t capacity;
	size_t length;
};

struct Vector* vector_create();
void vector_append(struct Vector* vector, int value);
int vector_pop(struct Vector* vector);
void vector_delete(struct Vector* vector);
char* vector_print(struct Vector* vector);

#endif
