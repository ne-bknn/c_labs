#include "gcd_lib.h"

void swap(int* a, int* b) {
	int t;
	t = *a;
	*a = *b;
	*b = t;
}

int gcd(int a, int b) {
	while (b) {
		a %= b;
		swap(&a, &b);
	}
	return a;
}
