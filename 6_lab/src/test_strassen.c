#include "strassen.h"
#include <stdlib.h>
#include <time.h>
int main() {
	printf("Benchmarking..\n");

	time_t t;
	time_t start, end;
	srand(time(&t));

	int n, m, p;
	n = 10;
	m = 15;
	p = 13;

	struct Matrix* a = matrix_generate(n, m);
	struct Matrix* b = matrix_generate(m, p);
	matrix_print(a);
	matrix_print(b);
	printf("========================\n");
	start = clock();
	struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	end = clock();
	printf("Time taken multiplying two 1000x1000 naively: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	printf("========================\n");
	matrix_print(r);
	matrix_free(r);
	printf("=======================\n================================\n");

	start = clock();
	const int k1 = padding_size(a->n, a->m);
	const int k2 = padding_size(b->n, b->m);
	const int k = MAX(k1, k2);
	matrix_pad(b, k);
	matrix_pad(a, k);
	r = matrix_strassen_multiply(a, b);
	end = clock();
	printf("Time taken multiplying using strassen: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	printf("==========================\n");
	matrix_print(r);

	matrix_free(a);
	matrix_free(b);
	matrix_free(r);

	return 0;
}

