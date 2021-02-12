#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

struct Row* create_row(size_t len) {
	struct Row* prow = malloc(sizeof(struct Row*));
	if (NULL == prow) {
		return NULL;
	}
	prow->length = len;
	prow->numbers = malloc(sizeof(double)*len);
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

void delete_matrix(struct Matrix* pmatrix) {
	struct Row* iter;
	for (iter = pmatrix->rows; iter != pmatrix->rows[pmatrix->length]; ++iter) {
		free(iter->numbers);
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

