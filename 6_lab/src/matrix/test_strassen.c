#include "strassen.h"

int main() {
	struct Matrix* a = matrix_generate(2, 2);
	struct Matrix* b = matrix_generate(2, 2);
	matrix_print(a);
	printf("=====================================\n");
	matrix_print(b);
	printf("=====================================\n");
	struct Matrix* r = matrix_naive_multiply(a, b);
	matrix_print(r);
	matrix_free(a);
	matrix_free(b);
	matrix_free(r);
	return 0;
}

