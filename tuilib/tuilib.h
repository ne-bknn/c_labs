#include <stddef.h>
#ifndef TUILIB_H
#define TUILIB_H

void msg_success(char *msg);
void msg_warn(char *msg);
void msg_error(char *msg);

typedef int (*callback)(void *callback_data, void *main_structure);

struct Op {
	const char *op_name;
	int n_args;
	int *arg_types;
	callback func;
};

void parse_input(void *main_structure, struct Op *ops, size_t n_ops);
#endif
