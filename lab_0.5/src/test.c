#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	int **a = calloc(5, sizeof(int));
	if (*a == NULL) {
		printf("Yes\n");
	} else {
		printf("No\n");
	}
	printf("%p\n", *a);
	printf("%p\n", NULL);
	return 0;
}
