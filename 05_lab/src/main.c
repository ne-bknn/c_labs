#include <stdio.h>
#include "lib.h"

int main() {
	int status_code = 1;
	while (status_code == 1) {
		struct Vector* pvec = create_vector();
		status_code = read_line(pvec);
		print_vector(pvec);
	}
}
