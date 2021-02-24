#include <stdio.h>

#define GREEN_BKGR "\e[42m"
#define BLACK_TEXT "\e[0;30m"

int main() {
	printf(BLACK_TEXT GREEN_BKGR);
	printf("text is green\n");
 	printf(GREEN_BKGR BLACK_TEXT);

	return 0;
}

