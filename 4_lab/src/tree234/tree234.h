#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "../tuilib.h"

enum Status {
	Success,
	SameKey,
};

struct NFInsertStatus {
	enum Status status;
	uint8_t a;
	uint8_t b;
};

struct BTree {
	struct Node* root;
	uint64_t n_elems;
	uint32_t n_saves;
};

struct Entry {
	uint64_t key;
	char* data;
};

struct Node {
	uint8_t is_leaf;
	struct Entry** keys;
	struct Node** subtrees;
	struct Node* parent;
	uint8_t n_entries;
	uint8_t n_subtrees;
};

// bytewise swap. stolen, probably works fine
#define byteswap(a, b, size)                \
	do {                                \
		size_t __size = (size);     \
      		char *__a = (a), *__b = (b);\
      		do {                        \
          		char __tmp = *__a;  \
          		*__a++ = *__b;      \
          		*__b++ = __tmp;     \
        	} while (--__size > 0);     \
    	} while (0)


static inline void nullcheck(void* ptr, const char* name) {
	if (NULL == ptr) {
		printf(ANSI_COLOR_RED "[!] %s is null" ANSI_COLOR_RESET, name);
	}
}

struct FindInorderResult {
	// pointer to a node with predecessor/successor
	struct Node* node;
	// index of an entry in a node with predecessor/successor
	uint8_t index;
};

struct FindInorderResult btree_find_predecessor(struct Node* starting_node, uint64_t key);
struct FindInorderResult btree_find_successor(struct Node* starting_node, uint64_t key);

void tree_merge();
void tree_shrink();

enum InsertStatus {
	InsertSuccess,
	InsertSameKey,
	InsertFatal
};

enum DeleteStatus {
	DeleteSuccess,
	DeleteNotFound
};

enum InsertStatus btree_insert(struct Node* root, struct BTree* btree, uint64_t key, char* data);
struct Node* btree_node_create();
struct Node* btree_node_split(struct Node* current_node, struct BTree* btree);
struct Entry* btree_search(struct BTree* btree, uint64_t key);
void btree_entry_print(struct Entry* entry);
enum DeleteStatus btree_delete(struct BTree* btree, uint64_t key);
void btree_save(struct BTree *btree, int index);
struct BTree* btree_create();
