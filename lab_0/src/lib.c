#include <stdio.h>
#include <stdbool.h>
#include "lib.h"

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s", msg);
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s", msg);
}

int get_int(int* a) {
	int n = 0;
	while (n == 0) {
		n = scanf("%d", a);
		if (n == 0) {
			print_warning("Error reading integer value");
			scanf("%*c");
		}
	}
	return n < 0 ? 0 : 1;
}

bool tuples_eq(struct Tuple* a, struct Tuple* b) {
	// a->b is a shorhand for (*a).b, as we accept pointers as arguments
	return (a->a == b->a && a->b == b->b);

}

struct Tuple count_and_multiply(int n) {
	struct Tuple answer = {0, 0}; 
	int temp = 0;
	while (n != 0) {
		temp = n % 10;
		if (temp % 2 == 1) {
			++answer.a;
			if (answer.b == 0) {
				answer.b = temp;
			} else {
				answer.b *= temp;
			}
		}
		n /= 10;
	}
	return answer;
}
