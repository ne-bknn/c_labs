/* BTree of degree 2 (2-3-4 tree)
 *
 * This materials were used to write this:
 * a) Introduction to Algorithms, 3rd edition, CLRS
 * b) https://www.programiz.com/dsa/b-tree
 * c) https://www.educative.io/page/5689413791121408/80001
 *
 * Supported ops:
 * 1) (enum Status) insert uint64_t char* - inserts in btree
 * 2) (enum Status) delete uint64_t - deletes element from btree
 * 3) (struct Entry*) get uint64_t - returns entry from btreee by key
 * 4) (void) print - prints the whole table in key order
 * 5) (struct Entry*) next uint64_t - returns elem with the smallest key greater than provided
 * 6) (void) save - saves the table in .dot format for debugging
 *
 * Timofey Bakunin, 2021
*/


#include <inttypes.h>
#include <stdlib.h>

#include "tree234.h"
#include "../utils.h"
#include "../tuilib.h"

// 12:18, 05.04. It is getting messy, if it will go on like that
// I will start breaking

// Inserts an entry in not full node
// Keeps entries sorted
//
// Do I have to sort pointers to subtrees, too?
// If this routine is used to insert in leafs, no, obviously
// If I'm using it in splitting, bad luck
//
// 0 on success, exits on major error
// (improper splitting, inserting in non-leaf)
// 1 on same key
// TODO: return enum status
struct NFInsertStatus btree_nonfull_insert(struct Node* current_node, struct Entry* new_entry) {
	struct NFInsertStatus status = {SameKey, 0, 0};
	if (current_node->n_entries == 3) {
		msg_error("Using internal insert with full node!");
		exit(1);
	}
	if (current_node->n_entries == 0) {
		// no entries so far - insert in first place
		current_node->keys[0] = new_entry;
		// TODO: should i allocate subtrees here?
		// only when we are inserting in non leaf...
		// there should be a check present uh
		// index, size
		status.a = 0;
		status.b = 1;
	} else if (current_node->n_entries == 1) {
		current_node->keys[1] = new_entry;
		if (current_node->keys[0]->key == new_entry->key) {
			return status;
		}
		if (current_node->keys[0]->key > new_entry->key) {
			byteswap(current_node->keys[0], current_node->keys[1], sizeof(struct Entry*));
			status.status = Success;
			status.a = 0;
		} else {
			status.a = 1;
		}
		status.b = 2;
	} else {
		// there are only two nodes
		if (current_node->keys[1]->key == new_entry->key) {
			return status;
		}
		if (current_node->keys[0]->key == new_entry->key) {
			return status;
		}
	
		current_node->keys[2] = new_entry;
		if (current_node->keys[1]->key > new_entry->key) {
			byteswap(current_node->keys[1], current_node->keys[2], sizeof(struct Entry*));
			if (current_node->keys[0]->key > current_node->keys[1]->key) {
				byteswap(current_node->keys[0], current_node->keys[1], sizeof(struct Entry*));
			}

		}
	}
	current_node->n_entries++;
	// this should return
	// {Success, Index, Size}
	status.status = Success;
	return status;	
}



struct Node* btree_node_split(struct Node* current_node, struct BTree* btree) {
	if (current_node->n_entries != 3) {
		msg_error("Trying to split not full node");
		exit(1);
	}
	struct Entry* median = current_node->keys[1];
	if (NULL != current_node->parent) {
		// splitting non-parent node
		//
		// darn i'm using nonfull_insert routine to split.. should i make it 
		// generic or create new one?
		//
		// cannot make it generic, will be too verbose for no reason for this usage
		// should nonfull_insert return something that 
		// will help me to sort pointers?
		btree_nonfull_insert(current_node->parent, median);

		struct Node* left = mknew(struct Node);
		struct Node* right = mknew(struct Node);
		
		left->is_leaf = current_node->is_leaf;
		right->is_leaf = current_node->is_leaf;
		
		// inserting in empty nodes
		btree_nonfull_insert(left, current_node->keys[0]);
		btree_nonfull_insert(right, current_node->keys[2]);

		// now we have two pointers that we need to insert in current_node->parent
		// btree_nonfull_insert should return index of insertion and the amount of elements
		// and lots of if elses.. i dunno how to make this more efficient, duh
		

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

struct BTree* tree_create() {
	struct BTree* btree = mknew(struct BTree);
	struct Node* root = mknew(struct Node);
	root->is_leaf = 1;
	root->keys = calloc(3, sizeof(struct Entry*));
	root->subtrees = calloc(4, sizeof(struct Node*));
	root->parent = NULL;
	root->n_entries = 0;
	btree->root = root;
	return btree;
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
	btree_nonfull_insert(current_node, new_entry);
	return 0;
}
