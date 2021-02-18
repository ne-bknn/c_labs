#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

struct Row* create_row(size_t len) {
	if (len == 0) {
		return NULL;
	}
	struct Row* prow = malloc(sizeof(struct Row));
	if (NULL == prow) {
		return NULL;
	}
	prow->length = len;
	prow->numbers = calloc(len, sizeof(double));
	prow->n_negative = 0;
	prow->n_positive = 0;
	if (NULL == prow->numbers) {
		free(prow);
		return NULL;
	}
	return prow;
}

struct Matrix* create_matrix(size_t len, size_t n_columns) {
	if ((len < 1) || (n_columns < 1)) {
		return NULL;
	}
	struct Matrix* pmatrix = malloc(sizeof(struct Matrix));
	if (NULL == pmatrix) {
		return NULL;
	}
	pmatrix->length = len;
	pmatrix->filled = 0;
	pmatrix->n_columns = n_columns;
	pmatrix->rows = malloc(sizeof(struct Row*)*len);
	if (NULL == pmatrix->rows) {
		free(pmatrix);
		return NULL;
	}
	return pmatrix;
}

void print_matrix(struct Matrix* pmatrix) {
	printf("   ");
	for (size_t j = 0; j < pmatrix->n_columns; ++j) {
		printf("%-12zu ", j);
	}
	printf("%-12s %-12s", "positive", "negative");
	printf("\n");
	for (size_t i = 0; i < pmatrix->length; ++i) {
		struct Row* iter = pmatrix->rows[i];
		printf("%zu] ", i);
		for (size_t j = 0; j < iter->length; ++j) {
			printf("%-12lf ", iter->numbers[j]);
		}
		for (size_t j = 0; j < pmatrix->n_columns - iter->length; ++j) {
			printf("%-12s ", "");
		}
		printf("%-12u %-12u\n", iter->n_positive, iter->n_negative);
	}
}

void delete_matrix(struct Matrix* pmatrix) {
	for (size_t i = 0; i < pmatrix->filled; ++i) {
		struct Row* iter = pmatrix->rows[i];	
		// same as *((*pmatrix).rows+sizeof(struct Row*)*i).numbers -- pointer to a row?
		free(iter->numbers);
		// same as *((*pmatrix).rows+sizeof(struct Row*)*i)
		free(iter);
	}
	free(pmatrix->rows);
	free(pmatrix);
}

void add_row_to_matrix(struct Matrix* pmatrix, struct Row *row, size_t index) {
	pmatrix->rows[index] = row;
	pmatrix->filled++;
}

int get_int(int* a) {
	int n = 0;
	while (n == 0) {
		n = scanf("%d", a);
		if (n == 0) { print_warning("Error reading integer value");
			scanf("%*[^\n]");
		}
	}
	return n < 0 ? 0 : 1;
}

int get_double(double* a) {
	int n = 0;
	while (n == 0) {
		n = scanf("%lf", a);
		if (n == 0) {
			print_warning("Error reading float value");
			scanf("%*[^\n]");
		}
	}
	return n < 0 ? 0 : 1;
}

void get_min_max(struct Matrix *pmatrix, int *index_neg_max, int *index_pos_max) {
	unsigned int max_positive = 0;
	unsigned int max_negative = 0;
	for (size_t i = 0; i < pmatrix->length; ++i) {
		// rows dereferenced once - got Row*
		if (pmatrix->rows[i]->n_positive >= max_positive) {
			print_debug("pmatrix->rows[i]->n_positive is %d; max_positive is %d\n", pmatrix->rows[i]->n_positive, max_positive);
			max_positive = pmatrix->rows[i]->n_positive;
			*index_pos_max = i;
		}
		if (pmatrix->rows[i]->n_negative >= max_negative) {
			print_debug("pmatrix->rows[i]->n_negativeis %d; max_negativeis %d\n", pmatrix->rows[i]->n_negative, max_positive);
			max_negative = pmatrix->rows[i]->n_negative;
			*index_neg_max= i;
		}
	}
	print_debug("*ppos: %d\n", *index_pos_max);
	print_debug("*pneg: %d\n", *index_neg_max);
}

void swap(struct Matrix *pmatrix, int index_pos_max, int index_neg_max) {
	print_debug("MAtrix length: %zu\n", pmatrix->length);
	struct Row *temp;
	// now there is a pointer to needed row
	temp = pmatrix->rows[index_pos_max];
	print_debug("index_pos_max: %d\n", index_pos_max);
	print_debug("temp pointer: %p\n", (void*)temp);
	// this does not work?????
	pmatrix->rows[index_pos_max] = pmatrix->rows[0];
	print_debug("%d\n", index_neg_max);
	pmatrix->rows[0] = temp;
	// lol this works alright
	//
	//
	// time for this part
	temp = pmatrix->rows[index_neg_max];
	print_debug("index_neg_max: %d\n", index_neg_max);
	print_debug("temp pointer: %p\n", (void*)temp);
	pmatrix->rows[index_neg_max] = pmatrix->rows[pmatrix->length-1];
	pmatrix->rows[pmatrix->length-1] = temp;
}

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s\n", msg);
}

void print_pointers(struct Matrix* pmatrix) {
	for (size_t i = 0; i < pmatrix->length; ++i) {
		print_debug("%p ", (void*)pmatrix->rows[i]);
	}
	printf("\n");
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

