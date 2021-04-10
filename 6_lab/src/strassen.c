#include "strassen.h"
#include <stdbool.h>

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

	const struct Matrix* restrict result = matrix_create(an, bm);
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

struct Matrix* matrix_add(const struct Matrix* restrict a, const struct Matrix* restrict b) {
	const struct Matrix* restrict res = matrix_create(a->n, a->m);
	for (int i = 0; i < a->n; ++i) {
		for (int j = 0; j < a->m; ++j) {
			res->elems[i*a->m+j] = b->elems[i*a->m+j] + a->elems[i*a->m+j];
		}
	}
	return res;
}

// will free a and b on autofree
struct Matrix* matrix_sub(const struct Matrix* restrict a, const struct Matrix* restrict b) {
	const struct Matrix* restrict res = matrix_create(a->n, a->m);
	for (int i = 0; i < a->n; ++i) {
		for (int j = 0; j < a->m; ++j) {
			res->elems[i*a->m+j] = -b->elems[i*a->m+j] + a->elems[i*a->m+j];
		}
	}
	return res;
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
	const struct Matrix* matrix = matrix_create(n, m);
	for (int i = 0; i < matrix->n; ++i) {
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

// two stolen functions
const int ceil_log2(unsigned long long x)
{
  static const unsigned long long t[6] = {
    0xFFFFFFFF00000000ull,
    0x00000000FFFF0000ull,
    0x000000000000FF00ull,
    0x00000000000000F0ull,
    0x000000000000000Cull,
    0x0000000000000002ull
  };

  int y = (((x & (x - 1)) == 0) ? 0 : 1);
  int j = 32;
  int i;

  for (i = 0; i < 6; i++) {
    int k = (((x & t[i]) == 0) ? 0 : j);
    y += k;
    x >>= k;
    j >>= 1;
  }

  return y;
}

const int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

// rows, columns, size to pad to
void matrix_pad(struct Matrix* a, int k) {
	// now we need to copy each row into padded matrix	
	double* b = calloc(k*k, sizeof(double));
	for (int i = 0; i < a->n; ++i) {
		memcpy(b+i*k, a->elems+i*a->m, a->m*sizeof(double));
	}
	free(a->elems);
	a->elems = b;
	a->n = k;
	a->m = k;
}

struct Matrix* matrix_strassen_multiply(struct Matrix *a, struct Matrix *b) {
	print_debug("Entered strassen multiply with size of block %lu", a->n);
	if (a->n <= 64) {
		struct Matrix* res = matrix_vecopt_multiply(a->elems, b->elems, a->n, a->m, b->n, b->m);
		return res;
	}

	struct Matrix* res = matrix_create(a->n, a->n);
	int k = a->n/2;

	struct Matrix* A11 = matrix_create(k, k);
	struct Matrix* A12 = matrix_create(k, k);
	struct Matrix* A21 = matrix_create(k, k);
	struct Matrix* A22 = matrix_create(k, k);
	struct Matrix* B11 = matrix_create(k, k);
	struct Matrix* B12 = matrix_create(k, k);
	struct Matrix* B21 = matrix_create(k, k);
	struct Matrix* B22 = matrix_create(k, k);

	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < k; ++j) {
			A11->elems[i*k+j] = a->elems[i*a->m+j];
			A12->elems[i*k+j] = a->elems[i*a->m + k + j];
			A21->elems[i*k+j] = a->elems[(k+i)*a->m+j];
			A22->elems[i*k+j] = a->elems[(k+i)*a->m+k+j];
			B11->elems[i*k+j] = b->elems[i*a->m+j];
			B12->elems[i*k+j] = b->elems[i*a->m + k + j];
			B21->elems[i*k+j] = b->elems[(k+i)*a->m+j];
			B22->elems[i*k+j] = b->elems[(k+i)*a->m+k+j];
		}
	}
	
	struct Matrix *temp1, *temp2;
	
	temp1 = matrix_sub(B12, B22);
	struct Matrix* P1 = matrix_strassen_multiply(A11, temp1);
	matrix_free(temp1);
	temp1 = matrix_add(A11, A12);
	struct Matrix* P2 = matrix_strassen_multiply(temp1, B22);
	matrix_free(temp1);
	temp1 = matrix_add(A21, A22);
	struct Matrix* P3 = matrix_strassen_multiply(temp1, B11);
	matrix_free(temp1);
	temp1 = matrix_sub(B21, B11);	
	struct Matrix* P4 = matrix_strassen_multiply(A22, temp1);
	matrix_free(temp1);
	temp1 = matrix_add(A11, A22);
	temp2 = matrix_add(B11, B22);
	struct Matrix* P5 = matrix_strassen_multiply(temp1, temp2);
	matrix_free(temp1);
	matrix_free(temp2);
	temp1 = matrix_sub(A12, A22);
	temp2 = matrix_add(B21, B22);
	struct Matrix* P6 = matrix_strassen_multiply(temp1, temp2);
	matrix_free(temp1);
	matrix_free(temp2);
	temp1 = matrix_sub(A11, A21);
	temp2 = matrix_add(B11, B12);
	struct Matrix* P7 = matrix_strassen_multiply(temp1, temp2);
	matrix_free(temp1);
	matrix_free(temp2);
	temp1 = matrix_add(P5, P4);
	temp2 = matrix_add(temp1, P6);
	matrix_free(temp1);
	struct Matrix* C11 = matrix_sub(temp2, P2);
	matrix_free(temp2);
	struct Matrix* C12 = matrix_add(P1, P2);
	struct Matrix* C21 = matrix_add(P3, P4);
	temp1 = matrix_add(P5, P1);
	temp2 = matrix_sub(temp1, P3);
	matrix_free(temp1);
	struct Matrix* C22 = matrix_sub(temp2, P7);
	matrix_free(temp2);

	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < k; ++j) {
			res->elems[i*a->n+j] = C11->elems[i*k+j];
			res->elems[i*a->n+j+k] = C12->elems[i*k+j];
			res->elems[(k+i)*a->n+j] = C21->elems[i*k+j];
			res->elems[(k+i)*a->n+k+j] = C22->elems[i*k+j];
		}
	}

	matrix_free(A11);
	matrix_free(A12);
	matrix_free(A21);
	matrix_free(A22);
	matrix_free(B11);
	matrix_free(B12);
	matrix_free(B21);
	matrix_free(B22);
	matrix_free(P1);
	matrix_free(P2);
	matrix_free(P3);
	matrix_free(P4);
	matrix_free(P5);
	matrix_free(P6);
	matrix_free(P7);
	matrix_free(C11);
	matrix_free(C12);
	matrix_free(C21);
	matrix_free(C22);

	return res;
}
