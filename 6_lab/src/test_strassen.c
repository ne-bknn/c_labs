#include "strassen.h"
#include <stdlib.h>

int main() {
	printf("Testing..\n");

	int n, m, p;
	n = 1000;
	m = 1000;
	p = 1000;

	struct Matrix* a = matrix_generate(n, m);
	struct Matrix* b = matrix_generate(m, p);

	// naive
	struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	
	// strassen
	const int k1 = padding_size(a->n, a->m);
	const int k2 = padding_size(b->n, b->m);
	const int k = MAX(k1, k2);

	matrix_pad(b, k);
	matrix_pad(a, k);
	struct Matrix *r1 = matrix_strassen_multiply(a, b);

	bool flag = false;	
	for (int i = 0; i < r->n*r->m; ++i) {
		if (r->elems[i]-r1->elems[(i/n)*k+(i%n)] > 0.000001) {
			printf("Found error, %lf, %lf, %d\n", r->elems[i], r1->elems[(i/n)*k+(i%n)], i);
			flag = true;
			break;
		}
	}
	
	if (!flag) {
		printf("Naive results and strassen results are the same\n");
	}
	matrix_free(a);
	matrix_free(b);
	matrix_free(r1);
	matrix_free(r);

	return 0;
}

