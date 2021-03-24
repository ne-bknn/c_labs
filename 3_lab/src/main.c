#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "utils.h"
#include "tuilib.h"
#include "table.h"

#define N_OPS 3

int main() {
	int insert_args[3]  = {0, 0, 1};
	int get_args[2] = {0, 0};
	callback func1 = tuilib_table_insert;
	struct Op insert_op = {"insert", 3, insert_args, func1, 0, "Inserts value in hashtable"};
	struct Op get_op = {"get", 2, get_args, &tuilib_item_get, 5, "Gets Item from table and prints it"};
	struct Op print_op = {"print", 0, NULL, &tuilib_table_print, 5, "Print the whole table"};
	struct Op ops[N_OPS] = {insert_op, get_op, print_op};

	struct Table* table = table_create();
	void* structure = (void*)table;

	int flag = 1;
	while (flag) {
		flag = parse_input(structure, ops, N_OPS);
	}

	table_delete(table);

	return 0;	
}
