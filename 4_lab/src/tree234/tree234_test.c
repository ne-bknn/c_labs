#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree234.h"
#include "../tuilib.h"

int main() {
	struct BTree* btree = btree_create();
	char* data = malloc(10);
	strcpy(data, "qwerqwera");
	btree_insert(btree->root, btree, 228, data);
	printf("Yay\n");
	return 0;
}
