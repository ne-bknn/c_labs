#include <inttypes.h>
#include "utils.h"
#include <stdbool.h>

#ifndef STRASSEN_PARALLEL_H
#define STRASSEN_PARALLEL_H

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

struct Matrix {
	double* restrict elems;
	// n rows
	int64_t n;
	// m columns
	int64_t m;
};

struct Tuple {
	struct Matrix* a;
	struct Matrix* b;
};

// n rows, m columns
static inline struct Matrix* matrix_create(int64_t n, int64_t m) {
	struct Matrix* matrix = mknew(struct Matrix);
	matrix->n = n;
	matrix->m = m;
	matrix->elems = (double*)calloc(n*m, sizeof(double));
	return matrix;
}

const int ceil_log2(unsigned long long x);
const int ipow(int base, int exp);
static inline const int padding_size(int n, int m) {
	int k = ipow(2, (ceil_log2(MAX(n, m))));
	return k;
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
struct Matrix* matrix_add(const struct Matrix* restrict a, const struct Matrix* restrict b);
struct Matrix* matrix_sub(const struct Matrix* restrict a, const struct Matrix* restrict b);
void matrix_pad(struct Matrix* a, int k);
struct Matrix* matrix_strassen_multiply(struct Matrix *a, struct Matrix *b);

#endif
