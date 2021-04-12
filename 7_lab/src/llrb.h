#ifndef LLRB_H_
#define LLRB_H_

#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>

enum Color
{
	black = 0,
	red = 1
};

// omg this guy I'm stealing code from
// typedefs variables that are subject to change
// in implementations, effectively creating an easy
// way to generic it. genialno.
typedef int t_key;
typedef int t_value;

struct LLRB {
	struct LLRBNode* root;
};

struct LLRBNode {
	t_key key;
	t_value value;
	enum Color color;
	struct LLRBNode *left;
	struct LLRBNode *right;
};

struct LLRBNode *tree_node_remove(struct LLRBNode *node, t_key key, struct LLRBNode *parent);
struct LLRBNode *tree_create_node(t_key key, t_value value);
// unsigned int	get_size(struct LLRBNode *node, t_key key);
t_value tree_get(struct LLRBNode *root, t_key key);
void tree_insert(struct LLRB *tree, t_key key, t_value value);
void tree_free(struct LLRBNode *root);
void tree_save(struct LLRBNode *root, int index);

#endif
