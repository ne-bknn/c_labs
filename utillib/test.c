#include "utils.h"

int main() {
	int *p;
	calloc_s(p, 1000, sizeof(int));
	for (int i = 0; i < 100; i++) {
		p[i] = i;
	}
	print_debug("some very long and useful string %d", p[69]);
	free_z(p);
	free_z(p);
	return 0;
}
