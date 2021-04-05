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
			byteswap((void*)current_node->keys[0], (void*)current_node->keys[1], sizeof(struct Entry*));
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
			byteswap((void*)current_node->keys[1], (void*)current_node->keys[2], sizeof(struct Entry*));
			if (current_node->keys[0]->key > current_node->keys[1]->key) {
				byteswap((void*)current_node->keys[0], (void*)current_node->keys[1], sizeof(struct Entry*));
				status.a = 0;
			} else {
				status.a = 1;
			}
		} else {
			status.a = 2;
		}
		status.b = 3;
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
		struct NFInsertStatus status = btree_nonfull_insert(current_node->parent, median);
		if (status.status == SameKey) {
			msg_error("SameKey in split, this should not happen");
			exit(1);
		}
		uint8_t index = status.a;
		uint8_t size = status.b;

		struct Node* left = btree_node_create();
		struct Node* right = btree_node_create();
		
		left->is_leaf = current_node->is_leaf;
		right->is_leaf = current_node->is_leaf;

		left->subtrees[0] = current_node->subtrees[0];
		if (NULL == left->subtrees[0]) {
			left->subtrees[0] = btree_node_create();
		}
		left->subtrees[0]->parent = left;
		
		left->subtrees[1] = current_node->subtrees[1];
		if (NULL == left->subtrees[1]) {
			left->subtrees[1] = btree_node_create();
		}
		left->subtrees[1]->parent = left;

		left->n_subtrees = 2;
		
		right->subtrees[0] = current_node->subtrees[2];
		if (NULL == right->subtrees[0]) {
			right->subtrees[0] = btree_node_create();
		}
		right->subtrees[0]->parent = right;

		right->subtrees[1] = current_node->subtrees[3];
		if (NULL == right->subtrees[1]) {
			right->subtrees[1] = btree_node_create();
		}
		right->subtrees[1]->parent = right;

		right->n_subtrees = 2;

		
		// inserting in empty nodes
		btree_nonfull_insert(left, current_node->keys[0]);
		btree_nonfull_insert(right, current_node->keys[2]);

		// now we have two pointers that we need to insert in current_node->parent
		// btree_nonfull_insert should return index of insertion and the amount of elements
		// and lots of if elses.. i dunno how to make this more efficient, duh
		// this should be rewritten TODO, but currently I do not understand how
		struct Node* parent = current_node->parent;
		left->parent = parent;
		right->parent = parent;
		if (size == 3) {
			if (index == 0) {
				// inserted in the beginning
				parent->subtrees[3] = parent->subtrees[2];
				parent->subtrees[2] = parent->subtrees[1];
				parent->subtrees[1] = right;
				parent->subtrees[0] = left;
				parent->n_subtrees = 4;
			} else if (index == 1) {
				// inserted in the middle
				parent->subtrees[3] = parent->subtrees[2];
				parent->subtrees[2] = right;
				parent->subtrees[1] = left;
				parent->n_subtrees = 4;
			} else if (index == 2) {
				// inserted in the end
				parent->subtrees[3] = right;
				parent->subtrees[2] = left;
				parent->n_subtrees = 4;
			} else {
				msg_error("Cannot handle such an index!");
				exit(1);
			}
			// yep i see the pattern, still dunno how to write it less verbose
		} else if (size == 2) {
			if (index == 0) {
				// inserted in the beginning
				parent->subtrees[2] = parent->subtrees[1];
				parent->subtrees[1] = right;
				parent->subtrees[0] = left;
				parent->n_subtrees = 3;
			} else if (index == 1) {
				parent->subtrees[2] = right;
				parent->subtrees[1] = left;
				parent->n_subtrees = 3;
			} else {
				msg_error("Could not handle such index!");
				exit(1);
			}
		} else if (size == 1) {
			if (index != 0) {
				msg_error("Could not handle such index!");
				exit(1);
			}
			parent->subtrees[0] = left;
			parent->subtrees[1] = right;
			parent->n_subtrees = 2;
		} else {
			msg_error("Cannot handle this size! node_split");
			exit(1);
		}
		return parent;
	} else {
		// Not-so-obvious idea from CLRS - we just overwriting root node in tree strucutre,
		// assigning new root as a parent of previous one
		// and call split again inside. cool.
		struct Node* new_root = btree_node_create();
		btree->root = new_root;
		new_root->subtrees[0] = current_node;
		new_root->n_subtrees = 1;
		current_node->parent = new_root;
		struct Node* parent = btree_node_split(current_node, btree);	
		return parent;
	}
	// TODO: memory is definitely leaking, we are not freeing splitted Node
}

struct BTree* btree_create() {
	struct BTree* btree = mknew(struct BTree);
	struct Node* root = btree_node_create();
	root->is_leaf = 1;
	root->parent = NULL;
	btree->root = root;
	btree->n_elems = 0;
	return btree;
}

struct Node* btree_node_create() {
	struct Node* node = mknew(struct Node);
	node->keys = calloc(3, sizeof(struct Entry*));
	node->subtrees = calloc(4, sizeof(struct Node*));
	node->n_entries = 0;
	return node;
}

enum InsertStatus btree_insert(struct Node* root, struct BTree* btree, uint64_t key, char* data) {
	struct Entry* new_entry = mknew(struct Entry);
	new_entry->key = key;
	new_entry->data = data;
	
	struct Node* current_node = root;

	while (!current_node->is_leaf || current_node->n_entries == 3) {
		// splitting while going down
		while (current_node->n_entries == 3) {
			current_node = btree_node_split(current_node, btree);
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
			if (current_node->keys[0]->key > key) {
				nullcheck(current_node->subtrees[0], "subtree[0]");
				current_node = current_node->subtrees[0];
			} else {
				nullcheck(current_node->subtrees[1], "subtree[1]");
				current_node = current_node->subtrees[1];
			}
		} else if (current_node->n_entries == 2) {
			if (current_node->keys[0]->key > key) {
				nullcheck(current_node->subtrees[0], "subtree[0]");
				current_node = current_node->subtrees[0];
			} else if (current_node->keys[0]->key == key) {
				free_z(data);
				free_z(new_entry);
				return InsertSameKey;
			} else if (current_node->keys[1]->key > key) {
				nullcheck(current_node->subtrees[1], "subtree[1]");
				current_node = current_node->subtrees[1];
			} else if (current_node->keys[1]->key == key) {
				free_z(data);
				free_z(new_entry);
				return InsertSameKey;
			} else {
				nullcheck(current_node->subtrees[2], "subtree[2]");
				current_node = current_node->subtrees[2];
			}
		} else {
			msg_error("Found unsplit node where should not!");
			print_debug("n_entries: %d\n", current_node->n_entries);
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
	btree->n_elems++;
	return InsertSuccess;
}

struct Entry* btree_search(struct BTree* btree, uint64_t key) {
	struct Node* current_node = btree->root;
	// rolling while true; continuing on new subtree, breaking
	// on find not found
	while (1) {
		uint8_t flag = 0;
		for (uint8_t i = 0; i < current_node->n_entries; ++i) {
			print_debug("%s", "i < current_node->n_entries");
			if (current_node->keys[i]->key > key) {
				// new subtree
				if (current_node->is_leaf) {
					// there is no such key
					return NULL;
				}
				current_node = current_node->subtrees[i];
				flag = 1;
				break;
			}
			if (current_node->keys[i]->key == key) {
				// found
				return current_node->keys[i];
			}
		}
		if (!flag) {
			current_node = current_node->subtrees[current_node->n_entries];
			if (NULL == current_node) {
				return NULL;
			}
		}
	}
}

void btree_entry_print(struct Entry* entry) {
	printf("Key: %"PRIu64", data: %s\n", entry->key, entry->data);
}

int btree_internal_write(struct Node* node, FILE *fp, int counter, int parent_index) {
	counter = counter + 1;
	int my_index = counter;
	if (NULL == fp) {
		msg_error("fp is null in btree_internal_write");
		exit(1);
	}
	if (node->n_entries == 1) {
		fprintf(fp, "\ta%03d [shape=record, label=\"{%"PRIu64"}\"];\n", counter, node->keys[0]->key);
	} else if (node->n_entries == 2) {
		fprintf(fp, "\ta%03d [shape=record, label=\"{%"PRIu64"|%"PRIu64"}\"];\n", counter, node->keys[0]->key, node->keys[1]->key);
	} else {
		fprintf(fp, "\ta%03d [shape=record, label=\"{%"PRIu64"|%"PRIu64"|%"PRIu64"}\"];\n", counter, node->keys[0]->key, node->keys[1]->key, node->keys[2]->key);
	}
	if (NULL != node->parent) {
		fprintf(fp, "\ta%03d -> a%03d;\n", counter+1, parent_index);
	}
	if (!node->is_leaf) {
		for (size_t i = 0; i < node->n_subtrees; ++i) {
			counter = btree_internal_write(node->subtrees[i], fp, counter, my_index);
		}
	}
	return counter;
}


void btree_save(struct BTree*  btree, int index) {
	FILE *fp;
	if (index > 1000) {
		msg_warn("index is greater than 1000, tweak btree_save");
	}
	char* filename = malloc(20);
	sprintf(filename, "save%d.dot", index);
	if (NULL != filename) {
		fp = fopen(filename, "w");
	} else {
		msg_error("Filename is NULL somehow...");
		exit(1);
	}
	fprintf(fp, "digraph {\n");

	btree_internal_write(btree->root, fp, 0, 0);
	fprintf(fp, "}");
	fclose(fp);
}
