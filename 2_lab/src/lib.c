#include "lib.h"
#include <stdio.h>

// Pretty printing helpers
void clear_screen() {
	printf("\e[1;1H\e[2J");
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

void print_active(char *msg) {
	printf(BLACK_TEXT GREEN_BKGR);
	printf("%s", msg);
	printf("\n");
	printf(GREEN_BKGR BLACK_TEXT);
}

