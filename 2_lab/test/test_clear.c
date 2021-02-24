#include <stdio.h>

int main() {
	printf("\e[1;1H\e[2J");
	printf("Hello, guys!\n");
	int n;
	scanf("%d", &n);
	printf("\e[1;1H\e[2J");
	printf("You entered: %d\n", n);
	scanf("%d", &n);
	return 0;
}
