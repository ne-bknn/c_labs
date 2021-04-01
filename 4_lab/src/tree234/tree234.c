#include <inttypes.h>

#include "../utils.h"
#include "../tuilib.h"
#include "tree234.h"

uint8_t tree_insert(struct Node* root, uint64_t key, char* data) {
	if (NULL == root) {
		root = mknew(struct Node);
		root->is_leaf = 1;
		root->keys = calloc(3, sizeof(struct Entry));
		root->subtrees = calloc(4, sizeof(struct Node*));
		root->parent = NULL;
		root->n_entries = 0;
	}

	struct Entry* new_entry = mknew(struct Entry);
	new_entry->key = key;
	new_entry->data = data;

	if (root->n_entries == 0) {

}
