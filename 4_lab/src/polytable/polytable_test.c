#include "../tuilib.h"
#include "polytable_interface.h"

int main() {
	struct Polytable *table = polytable_create();
	int insert_args[2] = {0, 1};
	int get_args[1] = {0};
	
	struct Op insert_op = {"insert", 2, insert_args, &tuilib_polytable_insert, 0, "Inserts value in hashtable"};
	struct Op get_op = {"get", 1, get_args, &tuilib_polytable_get, 0, "Gets value from hashtable"};
	struct Op print_op = {"print", 0, NULL, &tuilib_polytable_print, 0, "Prints the whole hashtable"};
	struct Op ops[3] = {insert_op, get_op, print_op};
	void* structure = (void*)table;
	int flag = 1;
	while (flag) {
		flag = parse_input(structure, ops, 3);
	}

	polytable_delete(table);

	return 0;
}
