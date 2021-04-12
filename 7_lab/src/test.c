#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "llrb.h"

int main() {
  struct LLRBNode *root = NULL;

  srandom((unsigned int)time(NULL));

  for (t_key i = 0; i < 1000; ++i) {
    tree_insert(&root, rand()%10000, i);
  }

  tree_save(root, 1);

  /*
  for (t_key i = 0; i < 50; ++i) {
  	printf("key: %lu, value: %lu\n",  i, tree_get(root, i));
  }
  */
  tree_free(root);
  return (0);
}
