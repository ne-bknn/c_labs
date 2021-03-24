#include "tuilib.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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
int parse_input(void *main_structure, struct Op *ops, size_t n_ops) {
	printf("> ");
	char command[64];
	int n = scanf("%63s", command);

	if (n < 0) {
		msg_success("Goodbye!");
		return 0;
	}
	
	if (strcmp(command, "exit") == 0) {
		msg_success("Goodbye!");
		return 0;
	}

	if (strcmp(command, "help") == 0) {
		for (size_t i = 0; i < n_ops; ++i) {
			printf("%s: %s\n", ops[i].op_name, ops[i].description);
		}
		printf("help: Display this message: help\n");
		printf("exit: Exit: exit\n");
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wunused-result"
		scanf("%*[^\n]");
		#pragma clang diagnostic pop
		return 1;
	}

	int op_index = -1;

	for (size_t i = 0; i < n_ops; ++i) {
		if (strcmp(ops[i].op_name, command) == 0) {
			op_index = i;
			print_debug("Op index: %lu", i);
			break;
		}
	}

	if (op_index == -1) {
		msg_warn("Unknown operation");
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wunused-result"
		scanf("%*[^\n]");
		#pragma clang diagnostic pop
		return 1;
	}

	struct Op current_op = ops[op_index];
	
	// structure for arguments of a func
	void **args = malloc(sizeof(void*)*current_op.n_args);
	
	// argument parsing and loading
	for (size_t i = 0; i < current_op.n_args; ++i) {
		if (current_op.arg_types[i] == 0) {
			int *temp = malloc(sizeof(int));
			n = scanf("%d", temp);
			print_debug("REad status int: %d", n);
			if (n != 1) {
				msg_warn("Could not parse command");
				#pragma clang diagnostic push
				#pragma clang diagnostic ignored "-Wunused-result"
				scanf("%*[^\n]");
				#pragma clang diagnostic pop
			
				free_z(temp);
				free_z(args);
				return 1;
			}
			args[i] = (void*)temp;
		} else if (current_op.arg_types[i] == 1) {
			// WHY???
			char* temp;
			n = scanf("%ms", &temp);
			print_debug("Got string: %s\n", temp);
			print_debug("Read status str: %d", n);
			if (n != 1) {
				msg_warn("Could not parse command");
				#pragma clang diagnostic push
				#pragma clang diagnostic ignored "-Wunused-result"
				scanf("%*[^\n]");
				#pragma clang diagnostic pop
			
				free_z(temp);
				free_z(args);
				return 1;
			}
			args[i] = (void*)temp;
		} else if (current_op.arg_types[i] == 2) {
			double *temp = malloc(sizeof(double));
			n = scanf("%lf", temp);
			print_debug("%d", n);
			if (n != 1) {
				msg_warn("Could not parse command");
				#pragma clang diagnostic push
				#pragma clang diagnostic ignored "-Wunused-result"
				scanf("%*[^\n]");
				#pragma clang diagnostic pop
			
				free_z(temp);
				free_z(args);
				return 1;
			}
			args[i] = (void*)temp;
		} else {
			msg_warn("Got unknown arg type");
			free_z(args);
			return 1;
		}
	}
	print_debug("%s\n", "Calling callback in parse_input");
	void* returned_value = current_op.func(args, main_structure);
	print_debug("%s\n", "Returned successfully");
	delete_args_arr(args, current_op.n_args);

	if (current_op.return_type == 0) {
		print_debug("%s\n", "zdes");
		printf("%d\n", *((int*)returned_value));
		print_debug("%s\n", "a teper zdes");
		free_z(returned_value);
	} else if (current_op.return_type == 1) {
		printf("%lf\n", *((double*)returned_value));
		free_z(returned_value);
	} else if (current_op.return_type == 3) {
		printf("%s\n", (char*)returned_value);
	} else if (current_op.return_type == 4) {
		printf("%s\n", (char*)returned_value);
		free_z(returned_value);
	} else if (current_op.return_type != 5) {
		msg_error("Unknown return type. This should not happen in runtime");
	}
	
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wunused-result"
	scanf("%*[^\n]");
	#pragma clang diagnostic pop
	return 1;
}

void delete_args_arr(void **args, size_t n_args) {
	for (size_t i = 0; i < n_args; ++i) {
		free_z(args[i]);
	}
	free_z(args);
}
