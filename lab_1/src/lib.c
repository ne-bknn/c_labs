#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

struct Row* create_row(size_t len) {
	struct Row* prow = malloc(sizeof(struct Row));
	if (NULL == prow) {
		return NULL;
	}
	prow->length = len;
	prow->numbers = calloc(len, sizeof(double));
	if (NULL == prow->numbers) {
		free(prow);
		return NULL;
	}
	return prow;
}

struct Matrix* create_matrix(size_t len) {
	struct Matrix* pmatrix = malloc(sizeof(struct Matrix));
	if (NULL == pmatrix) {
		return NULL;
	}
	pmatrix->length = len;
	pmatrix->rows = malloc(sizeof(struct Row*)*len);
	if (NULL == pmatrix->rows) {
		free(pmatrix);
		return NULL;
	}
	return pmatrix;
}

void print_matrix(struct Matrix* pmatrix) {
	for (size_t i = 0; i < pmatrix->length; ++i) {
		struct Row* iter = pmatrix->rows[i];
		for (size_t j = 0; j < iter->length; ++j) {
			printf("%-12lf ", iter->numbers[j]);
		}
		printf("\n");
	}
}

void delete_matrix(struct Matrix* pmatrix) {
	// add previous version of the code
	// with incrementing pointer
	// have no idea why this version works
	// and that did not
	for (size_t i = 0; i < pmatrix->length; ++i) {
		struct Row* iter = pmatrix->rows[i];	
		free(iter->numbers);
		// freed malloced numbers array, forgot that
		// struct is allocated on the heap too
		// spent half and hour debugging. bruh.
		free(iter);
	}
	free(pmatrix->rows);
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

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s\n", msg);
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

