#include "tuilib.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef ANSI_COLORS
#define ANSI_COLORS
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif

// Output utilities
void msg_success(char *msg) {
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
		if (strcmp(ops[i].op_name, command) == 0) {
			op_index = i;
			break;
		}
	}

	if (op_index == -1) {
		msg_warn("Unknown operation");
		return;
	}

	struct Op current_op = ops[op_index];
	void **args = malloc(sizeof(void*)*current_op.n_args);
	for (int i = 0; i < current_op.n_args; ++i) {
		if (current_op.arg_types[i] == 0) {
			args[i] = malloc(sizeof(int));
			scanf("%d", args[i]);
		} else if (current_op.arg_types[i] == 1) {
			args[i] = malloc(sizeof(char*));
			scanf("%ms", args[i]);
		} else if (current_op.arg_types[i] == 2) {
			args[i] = malloc(sizeof(double));
			scanf("%lf", args[i]);
		} else {
			msg_warn("Got unknown opcode");
			return;
		}
	}
	current_op.func(args, main_structure);
}

