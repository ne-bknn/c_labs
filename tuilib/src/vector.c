#include "vector.h"
#include <string.h>
#include <stdio.h>
#include "tuilib.h"
#include "utils.h"

#define uint unsigned int

struct Vector* vector_create() {
	struct Vector* vector = calloc(sizeof(struct Vector), 1);
	vector->data = calloc(sizeof(int), 10);
	vector->capacity = 10;
	vector->length = 0;
	return vector;
}

void vector_push(struct Vector* vector, int value) {
	print_debug("cap: %lu", vector->capacity);
	print_debug("len: %lu", vector->length);
	if (vector->capacity == vector->length) {
		vector->data = realloc(vector->data, sizeof(int)*(vector->capacity*2));
		vector->capacity = vector->capacity*2;
	}
	vector->data[vector->length] = value;
	vector->length += 1;
}

void* tuilib_vector_push(void** args, void* main_structure) {
	print_debug("%s", "Vector push");
	struct Vector* v = (struct Vector*)main_structure;
	int argument = *((int*)args[0]);
	vector_push(v, argument);
	return NULL;
}

int vector_pop(struct Vector* vector) {
	if (vector->length == 0) {
		return 0;
	}
	int value = vector->data[vector->length-1];
	vector->length = vector->length - 1;
	if (vector->length < (vector->capacity/2)) {
		vector->data = realloc(vector->data, sizeof(int)*(vector->capacity/2));
		vector->capacity = vector->capacity/2;
	}
	
	return value;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void* tuilib_vector_pop(void** args, void* main_structure) {
	print_debug("%s", "Vector pop");
#pragma clang diagnostic pop
	struct Vector* v = (struct Vector*)main_structure;
	int *value = malloc(sizeof(int));
	*value = vector_pop(v);
	return (void*)value;
}

void vector_delete(struct Vector* vector) {
	free(vector->data);
	free(vector);
}

char* vector_print(struct Vector* vector) {
	print_debug("%s", "Vector print");
	size_t needed = 0;
	for (uint i = 0; i < vector->length; ++i) {
		needed += snprintf(NULL, 0, " %d ", vector->data[i]);
	}

	char* buf = calloc(sizeof(char), needed+5);
	
	strncat(buf, "[", 1);
	for (uint i = 0; i < vector->length; ++i) {
		sprintf(buf+strlen(buf), " %d ", vector->data[i]);
	}
	strncat(buf, "]", 1);
	return buf;
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void* tuilib_vector_print(void** args, void* main_structure) {
#pragma clang diagnostic pop
	struct Vector* v = (struct Vector*)main_structure;
	char *repr = vector_print(v);
	return (void*)repr;
}
