#include <inttypes.h>
#include <stdlib.h>

#include "../utils.h"
#include "../tuilib.h"
#include "tree234.h"

// Inserts an entry in not full node
// 0 on success, exits on major error
// (improper splitting, inserting in non-leaf)
// 1 on same key
uint8_t tree_internal_insert(struct Node* current_node, struct Entry* new_entry) {
	if (current_node->n_entries == 3) {
		msg_error("Using internal insert with full node!");
		exit(1);
	}
	if (current_node->is_leaf == 0) {
		msg_error("Using internal insert on non-leaf node!");
		exit(1);
	}
	if (current_node->n_entries == 1) {
		current_node->keys[1] = new_entry;
		if (current_node->keys[0]->key == new_entry->key) {
			return 1;
		}
		if (current_node->keys[0]->key > new_entry->key) {
			byteswap(current_node->keys[0], current_node->keys[1], sizeof(struct Entry*));
		}
	} else {
		// so, there are two nodes. just bubbling it
		if (current_node->keys[1]->key == new_entry->key) {
			return 1;
		}
		if (current_node->keys[0]->key == new_entry->key) {
			return 1;
		}
	
		current_node->keys[2] = new_entry;
		if (current_node->keys[1]->key > new_entry->key) {
			byteswap(current_node->keys[1], current_node->keys[2], sizeof(struct Entry*));
			if (current_node->keys[0]->key > current_node->keys[1]) {
				byteswap(current_node->keys[0], current_node->keys[1], sizeof(struct Entry*));
			}

		}
	}
	current_node->n_entries++;
}

uint8_t tree_node_split(struct Node* current_node) {
	if (current_node->n_entries != 3) {
		msg_error("Trying to split not full node");
		exit(1);
	}
	struct Entry* median = current_node->keys[1];
	if (NULL != current_node->parent) {
		// splitting non-parent node
		tree_internal_insert(current_node->parent, median);
		byteswap(current_node->keys[1], current_node->keys[2], sizeof(struct Entry*));
		current_node->n_entries = 2;
	} else {
		// easiest case - we are splitting root
		// just create new root, set median entry to key
		// set first and second subtree to left node
		// and third and fourth subtrees to right node
		//
		// Smh I need to overwrite global pointer to 
		// root node. need a struct with a pointer to a node? or just pointer to a pointer to a node?
		struct Node* new_root = mknew(struct Node);
		new_root->is_leaf = 0;
		new_root->keys = calloc(3, sizeof(struct Entry*));
		new_root->subtrees = calloc(4, sizeof(struct Node*));
		new_root->parent = NULL;
		new_root->n_entries = 1;
		new_root->n_subtrees = 2;

		struct Node* left_node = mknew(struct Node);
		left_node->is_leaf = 0;
		left_node->keys = calloc(3, sizeof(struct Entry*));
		left_node->subtrees = calloc(4, sizeof(struct Node*));
		left_node->parent = new_root;
	}
}

uint8_t tree_insert(struct Node* root, uint64_t key, char* data) {
	if (NULL == root) {
		root = mknew(struct Node);
		root->is_leaf = 1;
		root->keys = calloc(3, sizeof(struct Entry*));
		root->subtrees = calloc(4, sizeof(struct Node*));
		root->parent = NULL;
		root->n_entries = 0;
	}

	struct Entry* new_entry = mknew(struct Entry);
	new_entry->key = key;
	new_entry->data = data;
	
	struct Node* current_node = root;

	while (!current_node->is_leaf) {
		// find leaf in which we are going
		// to insert
		// splitting occurs here, too
	}
	// we found the leaf and it is current_node
	// we didn't split the leaf, we have to do it here
	tree_internal_insert(current_node, new_entry);
	return 0;
}
