#include <stdio.h>
#include "lib.h"
#include <stdlib.h>

int main() {
	// m - rows, n - columns
	int n, m, status;
	get_int(&m);
	print_debug("M: %d\n", m);
	get_int(&n);
	print_debug("N: %d\n", n);
	
	struct Matrix* pmatrix = create_matrix(m, n);
	if (NULL == pmatrix) {
		print_error("Failed to create matrix.");
		exit(1);
	}
	int temp;
	int len = -1;
	for (int i = 0; i < m; ++i) {
		status = get_int(&len);
		if (status == 0) {
			print_error("Received unexpected EOF");
			exit(1);
		}

		if (len > n || len < 1) {
			print_error("Failed reading row length");
			exit(1);
		}
		print_debug("Length of the row: %d\n", len);
		struct Row* row = create_row(len);
		if (NULL == row) {
			print_error("Failed to create row");
			exit(1);
		}
		for (int j = 0; j < len; ++j) {
			status = get_int(&temp);
			if (status == 0) {
				print_error("Reiceved unexpected EOF");
				exit(1);
			}

			row->numbers[j] = (double) temp;
			if (temp < 0) {
				row->n_negative++;
			}
			if (temp > 0) {
				row->n_positive++;
			}
		}
		add_row_to_matrix(pmatrix, row, i);
	}

	print_matrix(pmatrix);
	int index_neg_max = 0, index_pos_max = 0;
	get_min_max(pmatrix, &index_neg_max, &index_pos_max);
	print_debug("index_neg_max in main: %d\n", index_neg_max);
	print_debug("index_pos_max in main: %d\n", index_pos_max);
	swap(pmatrix, index_neg_max, index_pos_max);
	print_debug("%s", "===============================\n");
	print_matrix(pmatrix);
	delete_matrix(pmatrix);
	return 0;	
}
