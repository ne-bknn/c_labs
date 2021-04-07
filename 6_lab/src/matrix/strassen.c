#include "strassen.h"

// Used to multiply two matrices with dims <= 4 (will move to 16 after debugging, maybe)
struct Matrix* matrix_naive_multiply(struct Matrix* a, struct Matrix* b) {
	if (a->m != b->n) {
		return NULL;
	}

	struct Matrix* result = matrix_create(a->n, b->m);
	for (int i = 0; i < a->n; ++i) {
		//result->elems[i] = row_create(b->m);
		for (int j = 0; j < b->m; ++j) {
			for (int k = 0; k < b->n; ++k) {
				result->elems[i][j] += a->elems[i][k]*b->elems[k][j];
			}
		}
	}
	return result;
}

// An attempt to optimize multiplication for vector operations
struct Matrix* matrix_vecopt_multiply(struct Matrix* a, struct Matrix* b, int64_t n1, int64_t m1, int64_t n2, int64_t m2) {
}

double rnd_double() {
	return (double)(rand()%1000)/1000;
}

struct Matrix* matrix_generate(int64_t n, int64_t m) {
	struct Matrix* matrix = matrix_create(n, m);
	for (int i = 0; i < matrix->n; ++i) {
		//matrix->elems[i] = row_create(m);
		for (int j = 0; j < matrix->m; ++j) {
			matrix->elems[i][j] = rnd_double();
		}
	}
	return matrix;
}

void matrix_print(struct Matrix* matrix) {
	printf("[");
	for (int i = 0; i < matrix->n; ++i) {
		printf("[");
		for (int j = 0; j < matrix->m; ++j) {
			printf("%.2f, ", matrix->elems[i][j]);
		}
		if (i+1 != matrix->n) {
			printf("],\n");
		}
	}
	printf("]\n");
}

void matrix_free(struct Matrix* matrix) {
	for (int i = 0; i < matrix->n; ++i) {
		free(matrix->elems[i]);
	}
	free(matrix->elems);
	free(matrix);
}
