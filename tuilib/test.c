#include <stdio.h> 
#include <stddef.h>
#include <stdlib.h>

#include "tuilib.h"

struct Stack {
	size_t capacity;
	size_t size;
	int *values;
};

// callback_data - first value is a pointer to an int
int push(void *callback_data, void *main_structure) {
	int argument = *(int*)(callback_data+0);
	struct Stack *structure = (struct Stack*) main_structure;
	if (structure->size == structure->capacity) {
		structure->values = realloc(structure->values, (structure->capacity)*2);
		if (NULL == structure->values) {
			return 0;
		}
		structure->capacity = structure->capacity * 2;
	}
	structure->values[structure->size] = argument;
	structure->size++;
	return 1;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int pop(void *callback_data, void *main_structure) {
#pragma clang diagnostic pop
	struct Stack *structure = (struct Stack*) main_structure;
	int last_elem = structure->values[structure->size];
	structure->size--;
	if (structure->size < structure->capacity / 2) {
		structure->values = realloc(structure->values, (structure->capacity)/2);
		if (NULL == structure->values) {
			msg_error("Could not realloc memory");
			return 0;
		}
	}
	printf("%d\n", last_elem);
	return 1;
}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
int print(void *callback_data, void *main_structure) {
#pragma clang diagnostic pop
	struct Stack *structure = (struct Stack*) main_structure;
	for (size_t i = 0; i < structure->size; ++i) {
		printf("%d ", structure->values[i]);
	}
	return 1;
}

int main() {
	int push_args[1] = {0};
	struct Op push_op = {"push", 1, push_args, &push};
	struct Op pop_op = {"pop", 0, NULL, &pop};
	struct Op print_op = {"print", 0, NULL, &print};
	struct Op ops[3] = {push_op, pop_op, print_op};
	int *values = malloc(5*sizeof(int));
	struct Stack s = {5, 0, values};
	while (1) {
		parse_input(&s, ops, 3);
	}
	return 0;
}
