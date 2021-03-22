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
void vector_push(struct Vector* vector, int value);
int vector_pop(struct Vector* vector);
void vector_delete(struct Vector* vector);
char* vector_print(struct Vector* vector);

void* tuilib_vector_push(void**args, void* main_structure);
void* tuilib_vector_print(void**args, void* main_structure);
void* tuilib_vector_pop(void**args, void* main_structure);
#endif
