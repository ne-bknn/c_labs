#include <inttypes.h>
#include "utils.h"

struct Matrix {
	double** elems;
	// n rows
	int64_t n;
	// m columns
	int64_t m;
};

// n rows, m columns
static inline struct Matrix* matrix_create(int64_t n, int64_t m) {
	struct Matrix* matrix = mknew(struct Matrix);
	matrix->n = n;
	matrix->m = m;
	double (*array) [n][m] = malloc(sizeof(double[n][m]));
	matrix->elems = array;
	return matrix;
}

static inline double* row_create(int64_t m) {
	return (double*)malloc(sizeof(double)*m);
}

struct Matrix* matrix_naive_multiply(struct Matrix* a, struct Matrix* b);
struct Matrix* matrix_generate(int64_t n, int64_t m);
void matrix_print(struct Matrix* matrix);
void matrix_free(struct Matrix* matrix);
