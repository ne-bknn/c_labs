#include "strassen.h"
#include <stdlib.h>
#include <time.h>
int main() {
	printf("Benchmarking..\n");

	time_t t;
	time_t start, end;
	srand(time(&t));

	int n, m, p;
	n = 1000;
	m = 1000;
	p = 1000;

	struct Matrix* a = matrix_generate(n, m);
	struct Matrix* b = matrix_generate(m, p);
	start = clock();
	struct Matrix* r = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
	end = clock();
	printf("Time taken multiplying two 1000x1000 naively: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	start = clock();
	const int k1 = padding_size(a->n, a->m);
	const int k2 = padding_size(b->n, b->m);
	const int k = MAX(k1, k2);
	matrix_pad(b, k);
	matrix_pad(a, k);
	struct Matrix *r1 = matrix_strassen_multiply(a, b);
	end = clock();
	printf("Time taken multiplying using strassen: %lf\n", (double)(end-start)/(double)(CLOCKS_PER_SEC));
	bool flag = false;	
	for (int i = 0; i < r->n*r->m; ++i) {
		if (r->elems[i]-r1->elems[(i/n)*k+(i%n)] > 0.00001) {
			printf("Found error, %lf, %lf, %d\n", r->elems[i], r1->elems[(i/1000)*1024+(i%1000)], i);
			flag = true;
			break;
		}
	}

	if (!flag) {
		printf("Naive results and strassen results are the same\n");
	}

	matrix_free(a);
	matrix_free(b);
	matrix_free(r);
	matrix_free(r1);

	return 0;
}

