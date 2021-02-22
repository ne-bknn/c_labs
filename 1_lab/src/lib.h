#include <stddef.h>

#ifndef LIB_H
#define LIB_H

#define DEBUG 0
#define print_debug(fmt, ...) \
	do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

// Row interface
struct Row {
	size_t length;
	unsigned int n_negative;
	unsigned int n_positive;
	double *numbers;
};

struct Row* create_row(size_t len);

// Matrix interface
struct Matrix {
	size_t n_columns;
	size_t length;
	size_t filled;
	struct Row **rows;
};

struct Matrix* create_matrix(size_t len, size_t n_columns);
void delete_matrix(struct Matrix* pmatrix);
void add_row_to_matrix(struct Matrix* pmatrix, struct Row *row, size_t index);
int get_int(int *a);
void print_warning(char msg[]);
void print_error(char msg[]);
void print_matrix(struct Matrix* pmatrix);
int get_double(double* a);
void get_min_max(struct Matrix *pmatrix, int *index_neg_max, int *index_pos_max);
void swap(struct Matrix *pmatrix, int index_neg_max, int index_pos_max);
void print_pointers(struct Matrix* pmatrix);
#endif
