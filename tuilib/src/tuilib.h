#include <stddef.h>

#ifndef ANSI_COLORS
#define ANSI_COLORS
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif

#ifndef TUILIB_H
#define TUILIB_H

void msg_success(char *msg);
void msg_warn(char *msg);
void msg_error(char *msg);

typedef void (*callback)(void **callback_data, void *main_structure);

// arg_types - 0 int, 1 double, 3 char*
// return_type - 0 int, 1 double, 3 const char*, 4 malloced char*, 5 void
struct Op {
	const char *op_name;
	size_t n_args;
	int *arg_types;
	callback func;
	int return_type;
	char* description;
};

void delete_args_arr(void **args, size_t n_args);
int parse_input(void *main_structure, struct Op *ops, size_t n_ops);
#endif
