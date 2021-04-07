#include <inttypes.h>
#include "utils.h"

struct Matrix {
	int64_t** elems;
	int64_t n;
	int64_t m;
};


// n rows, m columns
static inline struct Matrix* matrix_create(int64_t n, int64_t m) {
	struct Matrix* matrix = mknew(struct Matrix);
	matrix->n = n;
	matrix->m = m;
	matrix->elems = (int64_t**)malloc(sizeof(int64_t*)*m);
	return matrix;
}
