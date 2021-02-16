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
	for (size_t i = 0; i < pmatrix->length; ++i) {
		struct Row* iter = pmatrix->rows[i];	
		free(iter->numbers);
		free(iter);
	}
	free(pmatrix);
}

void add_row_to_matrix(struct Matrix* pmatrix, struct Row *row, size_t index) {
	pmatrix->rows[index] = row;
}

int get_int(int* a) {
	int n = 0;
	while (n == 0) {
		n = scanf("%d", a);
		if (n == 0) {
			print_warning("Error reading integer value");
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

void get_min_max(struct Matrix *pmatrix, struct Row **pneg, struct Row **ppos) {
	unsigned int max_positive = 0;
	unsigned int max_negative = 0;
	for (size_t i = 0; i < pmatrix->length; ++i) {
		if (pmatrix->rows[i]->n_positive >= max_positive) {
			max_positive = pmatrix->rows[i]->n_positive;
			*ppos = pmatrix->rows[i];
		}
		if (pmatrix->rows[i]->n_negative >= max_negative) {
			max_negative = pmatrix->rows[i]->n_negative;
			*pneg = pmatrix->rows[i];
		}
	}
}

void swap(struct Matrix *pmatrix, struct Row **pneg, struct Row **ppos) {
	struct Row *temp;
	temp = *ppos;
	*ppos = pmatrix->rows[0];
	pmatrix->rows[0] = temp;
	
	temp = *pneg;
	*pneg = pmatrix->rows[pmatrix->length];
	pmatrix->rows[pmatrix->length] = temp;
}

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s\n", msg);
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

