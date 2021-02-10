#include <stdio.h>

#include "gcd_lib.h"


int main() {
	int a, b, c = 0;
	scanf("%d", &a);
	scanf("%d", &b);
	c = gcd(a, b);
	printf("%d\n", c);
	return 0;
}
