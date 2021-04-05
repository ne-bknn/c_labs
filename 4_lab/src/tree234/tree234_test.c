#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "tree234.h"
#include "../tuilib.h"

int get_rnd_int(int lower_than) {
	if (lower_than == 0) {
		return 0;
	}
	return rand() % lower_than;
}

int main() {
	time_t t;
	time(&t);
	printf("Time: %lu\n", t);
	//srand((unsigned)t);
	srand(228);
	struct BTree* btree = btree_create();

	for (size_t i = 0; i < 10; ++i) {
		char* data = malloc(10);
		strcpy(data, "qwerqwera");
		int index = get_rnd_int(100);
		btree_insert(btree->root, btree, index, data);
		btree_save(btree, i);
		printf("Inserted %d\n", index);
	}

	for (size_t i = 0; i < 100; ++i) {
		struct Entry* entry = btree_search(btree, i);
		if (NULL != entry) {
			btree_entry_print(entry);
		} else {
			printf("%lu not found\n", i);
		}
	}
	return 0;
}
