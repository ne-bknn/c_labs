#include "strassen.h"
#include <stdlib.h>
#include <time.h>

int main() {
	int n, m, p;
	n = 10;
	m = 20;
	p = 30;

	struct Matrix* a = matrix_generate(n, m);
	struct Matrix* b = matrix_generate(m, p);

	matrix_print(a);
	printf("==============================================\n");
	matrix_print(b);
	printf("==============================================\n");
	struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	printf("Naively multiplied:\n");
	matrix_print(r);
	matrix_free(r);

	const int k1 = padding_size(a->n, a->m);
	const int k2 = padding_size(b->n, b->m);
	const int k = MAX(k1, k2);
	matrix_pad(b, k);
	matrix_pad(a, k);
	r = matrix_strassen_multiply(a, b);
	printf("==============================================\n");
	printf("Strassen multiplied:\n");
	matrix_print(r);

	matrix_free(a);
	matrix_free(b);
	matrix_free(r);
	return 0;
}
