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

void tree_left_steal();
void tree_right_steal();
void tree_merge();
void tree_shrink();

enum InsertStatus {
	InsertSuccess,
	InsertSameKey,
	InsertFatal
};
enum InsertStatus tree_insert(struct Node* root, uint64_t key, char* data);

struct Node* tree_node_split(struct Node* current_node);
void tree_delete();
void tree_save();
