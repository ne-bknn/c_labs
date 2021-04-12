#include "llrb.h"
#include <stdio.h>
#include "utils.h"

void tuilib_llrb_insert(void **callback_data, void* main_structure) {
	struct LLRB *tree = (struct LLRB*)main_structure;
	t_key *key = (t_key*)callback_data[0];
	t_value *value = (t_value*)callback_data[1];
	print_debug("key: %d, value: %d\n", *key, *value);
	tree_insert(tree, *key, *value);
}

void tuilib_llrb_delete(void **callback_data, void* main_structure) {
	struct LLRB *tree = (struct LLRB*)main_structure;
	struct LLRBNode *root = tree->root;
	t_key *key = (t_key*)callback_data[0];
	tree_node_remove(root, *key);
}

void tuilib_llrb_save(void **callback_data, void* main_structure) {
	struct LLRB *tree = (struct LLRB*)main_structure;
	struct LLRBNode *root = tree->root;
	tree_save(root, 1);
}

void tuilib_llrb_get(void **callback_data, void* main_structure) {
	struct LLRB *tree = (struct LLRB*)main_structure;
	struct LLRBNode *root = (struct LLRBNode*)tree->root;
	t_key *key = (t_key*)callback_data[0];
	t_value value = tree_get(root, *key);
	printf("Key: %d, Value: %d\n", *key, value);
}
