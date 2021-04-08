#include "strassen.h"
#include <stdlib.h>
#include <time.h>
int main() {
	time_t t;
	srand(time(&t));
	int n, m, p;
	n = 1000;
	m = 1000;
	p = 1000;
	printf("Generating matrices\n");
	struct Matrix* a = matrix_generate(n, m);
	struct Matrix* b = matrix_generate(m, p);
	time_t start, end;
	printf("Started calculating\n");
	//struct Matrix* res = matrix_create(n, p);
	start = clock();
	struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	//struct Matrix* r = matrix_naive_multiply(a, b);
	end = clock();
	printf("Time taken: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	matrix_free(a);
	matrix_free(b);
	matrix_free(r);
	return 0;
}

