#define DEBUG 1

#include <stdio.h> 
#include <stddef.h>
#include <stdlib.h>

#include "tuilib.h"
#include "vector.h"

int main() {
	int push_args[1] = {0};
	struct Op push_op = {"push", 1, push_args, &tuilib_vector_push, 5, "Pushes int to stack: push 5"};
	struct Op pop_op = {"pop", 0, NULL, &tuilib_vector_pop, 0, "Pops and prints element from top of stack: pop"};
	struct Op print_op = {"print", 0, NULL, &tuilib_vector_print, 4, "Prints actual vector: print"};
	struct Op ops[3] = {push_op, pop_op, print_op};
	struct Vector *v = vector_create();
	void* structure = (void*)v;
	
	int flag = 1;
	while (flag) {
		flag = parse_input(structure, ops, 3);
	}

	return 0;
}
