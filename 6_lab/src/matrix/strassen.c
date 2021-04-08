#include "strassen.h"

#define BLK_SIZE 128
#define min(a,b) (((a)<(b))?(a):(b))

// Used to multiply two matrices with dims <= 4 (will move to 16 after debugging, maybe)
#pragma clang optimize off
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
// still not vectorized enough
#pragma clang optimize on
struct Matrix* matrix_vecopt_multiply(const double* restrict a, const double* restrict b, int an, int am, int bn, int bm) {
	if (am != bn) {
		return NULL;
	}

	struct Matrix* restrict result = matrix_create(an, bm);
	int i1, i2, i3;
	for (int i = 0; i < an; ++i) {
		for (int j = 0; j < bm; ++j) {
			#pragma clang loop vectorize(enable)
			#pragma clang loop interleave(enable)
			for (int k = 0; k < bn; ++k) {
				i1 = i*bm+j;
				i2 = i*am+k;
				i3 = k*bm+j;
				result->elems[i1] += a[i2]*b[i3];
			}
		}
	}
	return result;
}

void matrix_add(double* restrict a, const double* restrict b, int n, int m) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			a[i*m+j] += b[i*m+j];
		}
	}
}

void matrix_sub(double* restrict a, const double* restrict b, int n, int m) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			a[i*m+j] += b[i*m+j];
		}
	}
}

#pragma clang optimize off
double rnd_double() {
	return (double)(rand()%1000)/1000;
}

#pragma clang optimize off
int rnd_int() {
	return rand()%40;
}

#pragma clang optimize off
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

#pragma clang optimize off
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
