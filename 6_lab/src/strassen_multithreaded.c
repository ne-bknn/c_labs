#define _GNU_SOURCE 

#include "strassen.h"
#include <stdbool.h>
#include <pthread.h>
#include <sched.h>

int parallelized = false;
int parallel_enabled = true;

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

void* matrix_strassen_wrapper(void* a) {
	struct Tuple* b = (struct Tuple*)a;
	void* ret = (void*)matrix_strassen_multiply(b->a, b->b);
	pthread_exit(ret);
}

struct Matrix* matrix_strassen_multiply(struct Matrix *a, struct Matrix *b) {
	pthread_t threads[7];
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
	
	struct Matrix *temp1, *temp2, *temp3, *temp4, *temp5, *temp6, *temp7, *temp8, *temp9, *temp10, *temp22, *temp33;
	
	bool local_parallelized = false;

	if (parallelized && !parallel_enabled) {
		parallelized = true;
	}

	if (!parallelized) {
		local_parallelized = true;
		parallelized = true;
	}

	struct Matrix* P1;

	struct Tuple *k1 = NULL, *k2 = NULL, *k3 = NULL, *k4 = NULL, *k5 = NULL, *k6 = NULL, *k7 = NULL;

	temp1 = matrix_sub(B12, B22);
	if (!local_parallelized) {
 		P1 = matrix_strassen_multiply(A11, temp1);
	} else {
		k1 = malloc(sizeof(struct Tuple));
		k1->a = A11;
		k1->b = temp1;
		pthread_create(&threads[0], NULL, matrix_strassen_wrapper, (void*)k1);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(0, &cpuset);
		pthread_setaffinity_np(threads[0], sizeof(cpuset), &cpuset);

	}

	temp2 = matrix_add(A11, A12);
	struct Matrix* P2;
	if (!local_parallelized) {
		P2 = matrix_strassen_multiply(temp2, B22);
	} else {
		k2 = malloc(sizeof(struct Tuple));
		k2->a = temp2;
		k2->b = B22;
		pthread_create(&threads[1], NULL, matrix_strassen_wrapper, (void*)k2);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(1, &cpuset);
		pthread_setaffinity_np(threads[1], sizeof(cpuset), &cpuset);

	}
	temp3 = matrix_add(A21, A22);
	struct Matrix* P3;
	if (!local_parallelized) {
		P3 = matrix_strassen_multiply(temp3, B11);
	} else {
		k3 = malloc(sizeof(struct Tuple));
		k3->a = temp3;
		k3->b = B11;
		pthread_create(&threads[2], NULL, matrix_strassen_wrapper, (void*)k3);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(2, &cpuset);
		pthread_setaffinity_np(threads[2], sizeof(cpuset), &cpuset);

	}

	temp4 = matrix_sub(B21, B11);	
	struct Matrix* P4;
	if (!local_parallelized) {
		P4 = matrix_strassen_multiply(A22, temp4);
	} else {
		k4 = malloc(sizeof(struct Tuple));
		k4->a = A22;
		k4->b = temp4;
		pthread_create(&threads[3], NULL, matrix_strassen_wrapper, (void*)k4);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(3, &cpuset);
		pthread_setaffinity_np(threads[3], sizeof(cpuset), &cpuset);

	}

	temp5 = matrix_add(A11, A22);
	temp6 = matrix_add(B11, B22);
	struct Matrix* P5;
	if (!local_parallelized) {
		P5 = matrix_strassen_multiply(temp5, temp6);
	} else {
		k5 = malloc(sizeof(struct Tuple));
		k5->a = temp5;
		k5->b = temp6;
		pthread_create(&threads[4], NULL, matrix_strassen_wrapper, (void*)k5);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(4, &cpuset);
		pthread_setaffinity_np(threads[4], sizeof(cpuset), &cpuset);

	}

	temp7 = matrix_sub(A12, A22);
	temp8 = matrix_add(B21, B22);
	struct Matrix* P6;
	if (!local_parallelized) {
		P6 = matrix_strassen_multiply(temp7, temp8);
	} else {
		k6 = malloc(sizeof(struct Tuple));
		k6->a = temp7;
		k6->b = temp8;
		pthread_create(&threads[5], NULL, matrix_strassen_wrapper, (void*)k6);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(5, &cpuset);
		pthread_setaffinity_np(threads[5], sizeof(cpuset), &cpuset);

	}

	temp9 = matrix_sub(A11, A21);
	temp10 = matrix_add(B11, B12);
	struct Matrix* P7;
	if (!local_parallelized) {
		P7 = matrix_strassen_multiply(temp9, temp10);
	} else {
		k7 = malloc(sizeof(struct Tuple));
		k7->a = temp9;
		k7->b = temp10;
		pthread_create(&threads[6], NULL, matrix_strassen_wrapper, (void*)k7);
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(6, &cpuset);
		pthread_setaffinity_np(threads[6], sizeof(cpuset), &cpuset);

	}

	if (local_parallelized) {
		pthread_join(threads[4], (void**)&P5);
		free_z(k5);
		pthread_join(threads[3], (void**)&P4);
		free_z(k4);
	}
	matrix_free(temp5);
	matrix_free(temp4);
	matrix_free(temp6);
	
	// definitely right

	temp22 = matrix_add(P5, P4);
	if (local_parallelized) {
		pthread_join(threads[5], (void**)&P6);
		free_z(k6);
	}
	matrix_free(temp7);
	matrix_free(temp8);
	
	temp33 = matrix_add(temp22, P6);
	matrix_free(temp22);
	if (local_parallelized) {
		pthread_join(threads[1], (void**)&P2);
		free_z(k2);
	}
	//free(temp2);

	struct Matrix* C11 = matrix_sub(temp33, P2);
	matrix_free(temp2);
	matrix_free(temp33);
	if (local_parallelized) {
		pthread_join(threads[0], (void**)&P1);
		free_z(k1);
	}
	matrix_free(temp1);

	struct Matrix* C12 = matrix_add(P1, P2);
	if (local_parallelized) {
		pthread_join(threads[2], (void**)&P3);
		free_z(k3);
	}
	matrix_free(temp3);
	struct Matrix* C21 = matrix_add(P3, P4);
	temp1 = matrix_add(P5, P1);
	temp2 = matrix_sub(temp1, P3);
	matrix_free(temp1);
	if (local_parallelized) {
		pthread_join(threads[6], (void**)&P7);
		free_z(k7);
	}
	matrix_free(temp10);
	matrix_free(temp9);
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
