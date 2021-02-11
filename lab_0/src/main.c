#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main() {
	tuple_list* Tuples = create_tuple_list(5);
	int input;
	// Do-while may be used but I think it's ugly
	printf("Enter your number: ");
	while (get_int(&input)) {
		if (malformed_input(input)) {
			print_warning("Integer should be greater or equal to zero");
			continue;
		}
		struct Tuple pair = multiply_and_count(input);
		add_tuple_to_list(Tuples, pair);
		printf("Successfully added your number, press Ctrl-D to finish.\nNumbers entered: %lu\n", Tuples->length);
		printf("Enter your number: ");
	}
	printf("\nOriginal sequence:\n");
	print_tuple_list(Tuples);
	printf("\nSorted sequence:\n");
	qsort(Tuples->tuples, Tuples->length, sizeof(struct Tuple), tuple_comparator);
	// bubble_sort_tuple_list(Tuples);
	print_tuple_list(Tuples);
	delete_tuple_list(Tuples);
	return 0;
}
