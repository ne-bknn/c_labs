#include <stdio.h>

typedef float m4x4_t __attribute__((matrix_type(4, 4)));

m4x4_t f(m4x4_t a, m4x4_t b, m4x4_t c) {
  return a + b * c;
}

int main() {
	m4x4_t a;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			a[i][j] = i*7+j;
		}
	}
	m4x4_t b;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			b[i][j] = i*13+j;
		}
	}
	m4x4_t c;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			c[i][j] = i*3+j;
		}
	}
	m4x4_t d = f(a, b, c);
	printf("%f\n", d[0][0]);
	return 0;
}

