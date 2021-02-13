#include <stdio.h>
#include "lib.h"
#include <stdlib.h>

int main() {
	// m - rows, n - columns
	int n, m;
	get_int(&m);
	get_int(&n);
	
	struct Matrix* pmatrix = create_matrix(m);
	if (NULL == pmatrix) {
		print_error("Could  not allocate memory");
		exit(1);
	}
	int temp;
	for (int i = 0; i < m; ++i) {
		int len;
		get_int(&len);
		struct Row* row = create_row(len);
		if (NULL == row) {
			print_error("Could not allocate memory");
			exit(1);
		}
		for (int j = 0; j < len; ++j) {
			get_int(&temp);
			row->numbers[j] = (double) temp;
		}
		add_row_to_matrix(pmatrix, row, i);
	}
	print_matrix(pmatrix);
	delete_matrix(pmatrix);
	return 0;	
}
