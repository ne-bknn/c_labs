#include "tuilib.h"
#include <stdio.h>
#include <stddef.h>

#ifndef ANSI_COLORS
#define ANSI_COLORS
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif

// Output utilities
void msg_sucсess(char *msg) {
	printf(ANSI_COLOR_GREEN);
	printf("[+] %s", msg);
	printf(ANSI_COLOR_RESET);
	printf("\n");
}

void msg_warn(char *msg) {
	printf(ANSI_COLOR_YELLOW);
	printf("[-] %s", msg);
	printf(ANSI_COLOR_RESET);
	printf("\n");
}

void msg_error(char *msg) {
	printf(ANSI_COLOR_RED);
	printf("[!] %s", msg);
	printf(ANSI_COLOR_RESET);
	printf("\n");
}

// Parsing procedure

void parse_input(void *main_structure, struct Op *ops, size_t n_ops) {
	printf("> ");
	char command[64];
	scanf("%63s", command);
	int op_index = -1;

	for (size_t i = 0; i < n_ops; ++i) {
		if (strcmp(ops[i]->op_name, command) == 0) {
			op_index = i;
			break;
		}
	}

	if (op_index == -1) {
		msg_warn("Unknown operation");
		return;
	}

}
