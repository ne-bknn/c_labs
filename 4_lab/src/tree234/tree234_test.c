#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree234.h"
#include "../tuilib.h"

int main() {
	struct BTree* btree = btree_create();
	for (size_t i = 0; i < 10; ++i) {
		char* data = malloc(10);
		strcpy(data, "qwerqwera");
		btree_insert(btree->root, btree, i*3, data);
	}
	for (size_t i = 0; i < 10; ++i) {
		struct Entry* entry = btree_search(btree, i*3);
		if (NULL != entry) {
			btree_entry_print(entry);
		} else {
			printf("%lu not found", i*3);
		}
	}
	return 0;
}
