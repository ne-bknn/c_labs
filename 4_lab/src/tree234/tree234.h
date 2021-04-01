#include <inttypes.h>

struct Entry {
	uint64_t key;
	char* data;
};

struct Node {
	uint8_t is_leaf;
	struct Entry* keys;
	struct Node** subtrees;
	struct Node* parent;
	uint8_t n_entries;
};

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

void tree_left_steal();
void tree_right_steal();
void tree_merge();
void tree_shrink();

uint8_t tree_insert(struct Node* root, uint64_t key, char* data);
void tree_split();
void tree_delete();
void tree_save();
