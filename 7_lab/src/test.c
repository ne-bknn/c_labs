#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "llrb.h"
#include "utils.h"

int main() {
  srandom((unsigned int)time(NULL));
   
  struct LLRB* tree = mknew(struct LLRB);
  tree->root = NULL;
  for (t_key i = 0; i < 1000; ++i) {
    tree_insert(tree, rand()%10000, i);
  }

  tree_save(tree->root, 1);

  /*
  for (t_key i = 0; i < 50; ++i) {
  	printf("key: %lu, value: %lu\n",  i, tree_get(root, i));
  }
  */
  tree_free(tree->root);
  return (0);
}
