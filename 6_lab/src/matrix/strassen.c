#include "strassen.h"

// Used to multiply two matrices with dims <= 4 (will move to 16 after debugging, maybe)
struct Matrix* matrix_naive_multiply(struct Matrix* restrict a, struct Matrix* restrict b) {
	if (a->m != b->n) {
		return NULL;
	}

	struct Matrix* restrict result = matrix_create(a->n, b->m);

	for (int i = 0; i < a->n; ++i) {
		for (int j = 0; j < b->m; ++j) {
			for (int k = 0; k < b->n; ++k) {
				result->elems[i*a->n + j] += (a->elems[i*a->m+k])*(b->elems[k*b->m+j]);
			}
		}
	}
	return result;
}

// An attempt to optimize multiplication for vector operations
struct Matrix* matrix_vecopt_multiply(double* restrict a, double* restrict b, int64_t an, int64_t am, int64_t bn, int64_t bm) {
	if (am != bn) {
		return NULL;
	}

	struct Matrix* restrict result = matrix_create(an, bm);

	for (int i = 0; i < an; ++i) {
		for (int j = 0; j < bm; ++j) {
			for (int k = 0; k < bn; ++k) {
				result->elems[i*bm + j] += (a[i*am+k])*(b[k*bm+j]);
			}
		}
	}
	return result;
}

double rnd_double() {
	return (double)(rand()%1000)/1000;
}

int rnd_int() {
	return rand()%40;
}

struct Matrix* matrix_generate(int64_t n, int64_t m) {
	struct Matrix* matrix = matrix_create(n, m);
	for (int i = 0; i < matrix->n; ++i) {
		//matrix->elems[i] = row_create(m);
		for (int j = 0; j < matrix->m; ++j) {
			matrix->elems[i*m+j] = rnd_double();
		}
	}
	return matrix;
}

void matrix_print(struct Matrix* matrix) {
	printf("[");
	for (int i = 0; i < matrix->n; ++i) {
		printf("[");
		for (int j = 0; j < matrix->m; ++j) {
			printf("%.2f, ", matrix->elems[i*matrix->m+j]);
		}
		if (i+1 != matrix->n) {
			printf("],\n");
		} else {
			printf("]");
		}
	}
	printf("]\n");
}

void matrix_free(struct Matrix* matrix) {
	free(matrix->elems);
	free(matrix);
}
