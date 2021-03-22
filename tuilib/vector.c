#include "vector.h"
#include <string.h>
#include <stdio.h>

#define uint unsigned int

struct Vector* vector_create() {
	struct Vector* vector = calloc(sizeof(struct Vector), 1);
	vector->data = calloc(sizeof(int), 10);
	vector->capacity = 10;
	vector->length = 0;
	return vector;
}

void vector_append(struct Vector* vector, int value) {
	if (vector->capacity == vector->length) {
		vector->data = realloc(vector->data, sizeof(int)*(vector->capacity*2));
		vector->capacity = vector->capacity*2;
	}
	vector->data[vector->length] = value;
	vector->length += 1;
}

int vector_pop(struct Vector* vector) {
	int value = vector->data[vector->length-1];
	vector->length = vector->length - 1;
	if (vector->length < (vector->capacity/2)) {
		vector->data = realloc(vector->data, sizeof(int)*(vector->capacity/2));
		vector->capacity = vector->capacity/2;
	}

	return value;
}

void vector_delete(struct Vector* vector) {
	free(vector->data);
	free(vector);
}

char* vector_print(struct Vector* vector) {
	size_t needed = 0;
	for (uint i = 0; i < vector->length; ++i) {
		needed += snprintf(NULL, 0, " %d ", vector->data[i]);
	}

	char* buf = calloc(sizeof(char), needed+5);
	
	strncat(buf, "[", 1);
	for (uint i = 0; i < vector->length; ++i) {
		sprintf(buf+strlen(buf), " %d ", vector->data[i]);
	}
	strncat(buf, "]\n", 2);
	return buf;
}
