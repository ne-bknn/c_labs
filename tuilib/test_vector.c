#include "vector.h"
#include <stdio.h>

int main() {
	struct Vector* v = vector_create();
	for (size_t i = 1; i < 20; ++i) {
		printf("len: %lu, cap: %lu\n", v->length, v->capacity);
		char* repr = vector_print(v);
		printf("%s", repr);
		vector_append(v, i);
		free(repr);
	}
	int k;
	for (size_t i = 1; i < 15; ++i) {
		printf("len: %lu, cap: %lu\n", v->length, v->capacity);
		char* repr = vector_print(v);
		printf("%s", repr);
		k = vector_pop(v);
		printf("k: %d\n", k);
		free(repr);
	}
	vector_delete(v);
	return 0;
}
