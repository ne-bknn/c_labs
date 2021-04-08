#include <inttypes.h>
#include "utils.h"

struct Matrix {
	double* restrict elems;
	// n rows
	int64_t n;
	// m columns
	int64_t m;
};

// n rows, m columns
static inline struct Matrix* matrix_create(int64_t n, int64_t m) {
	print_debug("Allocating matrix of size %lu*%lu", n, m);
	struct Matrix* matrix = mknew(struct Matrix);
	matrix->n = n;
	matrix->m = m;
	matrix->elems = (double*)calloc(n*m, sizeof(double));
	return matrix;
}

static inline double* row_create(int64_t m) {
	return (double*)malloc(sizeof(double)*m);
}

int rnd_int();
struct Matrix* matrix_naive_multiply(struct Matrix* a, struct Matrix* b);
struct Matrix* matrix_generate(int64_t n, int64_t m);
struct Matrix* matrix_vecopt_multiply(const double* a, const double *b, int an, int am, int bn, int bm);
void matrix_print(struct Matrix* matrix);
void matrix_free(struct Matrix* matrix);
void matrix_add(double* restrict a, const double* restrict b, int n, int m);
void matrix_sub(double* restrict a, const double* restrict b, int n, int m);
