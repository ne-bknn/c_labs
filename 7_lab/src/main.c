#include "tuilib.h"
#include "interface.h"
#include "llrb.h"
#include "utils.h"

#define N_OPS 4

int main() {
	int *save_args = NULL;
	int insert_args[2] = {0, 0};
	int delete_args[1] = {0};

	struct Op save_op = {"save", 0, save_args, &tuilib_llrb_save, 0, "Saves tree to graph"};
	struct Op insert_op = {"insert", 2, insert_args, &tuilib_llrb_insert, 0, "Inserts KV in atree"};
	struct Op delete_op = {"delete", 1, delete_args, &tuilib_llrb_delete, 0, "Deletes key from tree"};
	struct Op get_op = {"get", 1, delete_args, &tuilib_llrb_get, 0, "Gets an item from tree"};

	struct Op operations[N_OPS] = {save_op, insert_op, delete_op, get_op};

	struct LLRB* tree = mknew(struct LLRB);
	tree->root = NULL;
	void* structure = (void*)tree;

	int flag = 1;
	while (flag) {
		flag = parse_input(structure, operations, N_OPS);
	}

	tree_free(tree->root);
	return 0;
}
