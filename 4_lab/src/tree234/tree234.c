#include <inttypes.h>
#include <stdlib.h>

#include "../utils.h"
#include "../tuilib.h"
#include "tree234.h"

// Inserts an entry in not full node
// 0 on success, exits on major error
// (improper splitting, inserting in non-leaf)
// 1 on same key
uint8_t btree_nonfull_insert(struct Node* current_node, struct Entry* new_entry) {
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

struct Node* btree_node_split(struct Node* current_node, struct BTree* btree) {
	if (current_node->n_entries != 3) {
		msg_error("Trying to split not full node");
		exit(1);
	}
	struct Entry* median = current_node->keys[1];
	if (NULL != current_node->parent) {
		// splitting non-parent node
		btree_nonfull_insert(current_node->parent, median);
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
		
		// Not-so-obvious idea from CLRS - we just overwriting root node in tree strucutre,
		// assigning new root as a parent of previous one
		// and call split again inside. cool.
		struct Node* new_root = mknew(struct Node);
		btree->root = new_root;
		current_node->parent = new_root;
		struct Node* parent = btree_node_split(current_node, btree);	
		return parent;
		// looks so much prettier

		/*
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
		*/
	}
}

struct Node* tree_create() {
	struct Node* root = mknew(struct Node);
	root->is_leaf = 1;
	root->keys = calloc(3, sizeof(struct Entry*));
	root->subtrees = calloc(4, sizeof(struct Node*));
	root->parent = NULL;
	root->n_entries = 0;

}

enum InsertStatus tree_insert(struct Node* root, uint64_t key, char* data) {
	struct Entry* new_entry = mknew(struct Entry);
	new_entry->key = key;
	new_entry->data = data;
	
	struct Node* current_node = root;

	while (!current_node->is_leaf) {
		// splitting while going down
		if (current_node->n_entries == 3) {
			current_node = tree_node_split(current_node);
		}
		
		// searching
		if (current_node->n_entries == 1) {
			if (current_node->keys[0]->key == key) {
				free_z(data);
				free_z(new_entry);
				return InsertSameKey;
			}
			// TODO: null checks
			// can null be here? didn't we create needed subtrees during splitting?
			// null in subtree can be in leaf only and if it is a leaf
			// we do not enter this loop
			if (current_node->keys[0]-> key < key) {
				nullcheck(current_node->subtrees[0], "subtree[0]");
				current_node = current_node->subtrees[0];
			} else {
				nullcheck(current_node->subtrees[1], "subtree[1]");
				current_node = current_node->subtrees[1];
			}
		} else if (current_node->n_entries == 2) {
			if (current_node->keys[0]->key < key) {
				nop();
			} else if (current_node->keys[0]->key == key) {
				free_z(data);
				free_z(new_entry);
				return InsertSameKey;
			} else if (current_node->keys[1]->key < key) {
				nop();
			} else if (current_node->keys[1]->key == key) {
				free_z(data);
				free_z(new_entry);
				return InsertSameKey;
			} else {
				nop();
			}
		} else {
			msg_error("Found unsplit node where should not!");
			exit(1);
		}
	}
	// we found the leaf and it is current_node
	// we didn't split the leaf, we have to do it here
	//
	// 04.04, 3 AM: actually no, i need to split leaf in
	// while loop so there will be no need to reimplement
	// searching the actual leaf
	tree_internal_insert(current_node, new_entry);
	return 0;
}
