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
	//matrix_print(a);
	printf("========================\n");
	//matrix_print(b);
	printf("=======================\n");
	time_t start, end;
	printf("Started calculating\n");
	start = clock();
	//struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	matrix_add(a->elems, b->elems, a->n, a->m);
	end = clock();
	printf("Time taken: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	//matrix_print(a);
	//matrix_print(a);
	//matrix_print(b);
	//matrix_print(r);
	matrix_free(a);
	matrix_free(b);
	//matrix_free(r);
	return 0;
}

