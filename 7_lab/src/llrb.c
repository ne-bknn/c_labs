// https://github.com/sebastiencs/red-black-tree is used as a reference
// Reference is licensed under The Unlicense

#include <stdio.h>
#include <string.h>

#include "llrb.h"
#include "tuilib.h"
#include "utils.h"

static inline int is_red(struct LLRBNode *node) {
	return ((node) ? (node->color == red) : (0));
}

static inline int tree_key_comparator(unsigned int key1, unsigned int key2) {
	return ((key1 == key2) ? (0) : ((key1 < key2) ? (-1) : (1)));
}

static void tree_node_flip_color(struct LLRBNode *node) {
	node->color = !(node->color);
	node->left->color = !(node->left->color);
	node->right->color = !(node->right->color);
}

static struct LLRBNode *tree_rotate_left(struct LLRBNode *left) {
	struct LLRBNode *right;

	if (!left) {
		return ((struct LLRBNode *)NULL);
	}

	right = left->right;
	left->right = right->left;
	right->left = left;
	right->color = left->color;
	left->color = red;
	return (right);
}

static struct LLRBNode *tree_rotate_right(struct LLRBNode *right) {
	struct LLRBNode *left;

	if (!right) {
		return ((struct LLRBNode *)NULL);
	}
	left = right->left;
	right->left = left->right;
	left->right = right;
	left->color = right->color;
	right->color = red;
	return (left);
}

struct LLRBNode *tree_create_node(t_key key, t_value value) {
	struct LLRBNode *new = (struct LLRBNode *)malloc(sizeof(*new));

	if (new == (struct LLRBNode *)NULL) {
		return ((struct LLRBNode *)NULL);
	}

	new->key = key;
	new->value = value;
	new->color = red;
	new->left = (struct LLRBNode *)NULL;
	new->right = (struct LLRBNode *)NULL;
	return (new);
}

static struct LLRBNode *tree_internal_insert(struct LLRBNode *node, t_key key, t_value value) {
	int res;

	if (!node) {
		return (tree_create_node(key, value));
	}
	res = tree_key_comparator(key, node->key);
	if (res == 0) {
		node->value = value;
	} else if (res < 0) {
		node->left = tree_internal_insert(node->left, key, value);
	} else {
		node->right = tree_internal_insert(node->right, key, value);
	}

	if (is_red(node->right) && !is_red(node->left)) {
		node = tree_rotate_left(node);
	}
	if (is_red(node->left) && is_red(node->left->left)) {
		node = tree_rotate_right(node);
	}
	if (is_red(node->left) && is_red(node->right)) {
		tree_node_flip_color(node);
	}
	return (node);
}

void tree_insert(struct LLRB *tree, t_key key, t_value value) {
	tree->root = tree_internal_insert(tree->root, key, value);
	if (tree->root) {
		tree->root->color = black;
	}
}

t_value tree_get(struct LLRBNode *node, t_key key) {
	int cmp;

	while (node) {
		if (!(cmp = tree_key_comparator(key, node->key))) {
			return (node->value);
		}
		if (cmp < 0) {
			if (node->left != NULL) {
				node = node->left;
			} else {
				return 0;
			}
		} else {
			if (node->right != NULL) {
				node = node->right;
			} else {
				return 0;
			}
		}
	}
	return (0);
}

static struct LLRBNode *tree_get_min(struct LLRBNode *node) {
	if (!node) {
		return ((struct LLRBNode *)NULL);
	}
	while (node->left) {
		node = node->left;
	}
	return (node);
}

struct LLRBNode *tree_fixup(struct LLRBNode *node) {
	if (is_red(node->right)) {
		node = tree_rotate_left(node);
	}
	if (is_red(node->left) && is_red(node->left->left)) {
		node = tree_rotate_right(node);
	}
	if (is_red(node->left) && is_red(node->right)) {
		tree_node_flip_color(node);
	}
	return (node);
}

static struct LLRBNode *tree_internal_red_to_left(struct LLRBNode *node) {
	tree_node_flip_color(node);
	if (node && node->right && is_red(node->right->left)) {
		node->right = tree_rotate_right(node->right);
		node = tree_rotate_left(node);
		tree_node_flip_color(node);
	}
	return (node);
}

static struct LLRBNode *tree_internal_red_to_right(struct LLRBNode *node) {
	tree_node_flip_color(node);
	if (node && node->left && is_red(node->left->left)) {
		node = tree_rotate_right(node);
		tree_node_flip_color(node);
	}
	return (node);
}

static struct LLRBNode *tree_remove_min(struct LLRBNode *node) {
	if (!node) {
		return ((struct LLRBNode *)NULL);
	}
	if (!node->left) {
		free(node);
		return ((struct LLRBNode *)NULL);
	}
	if (!is_red(node->left) && !is_red(node->left->left)) {
		node = tree_internal_red_to_left(node);
	}
	node->left = tree_remove_min(node->left);
	return (tree_fixup(node));
}

static struct LLRBNode *tree_internal_remove(struct LLRBNode *node, t_key key) {
	struct LLRBNode *tmp;

	if (!node) {
		return ((struct LLRBNode *)NULL);
	}

	if (tree_key_comparator(key, node->key) == -1) {
		if (node->left) {
			if (!is_red(node->left) && !is_red(node->left->left)) {
				node = tree_internal_red_to_left(node);
			}
			node->left = tree_node_remove(node->left, key);
		}
	} else {
		if (is_red(node->left)) {
			node = tree_rotate_right(node);
		}
		if (!tree_key_comparator(key, node->key) && !node->right) {
			free(node);
			return ((struct LLRBNode *)0);
		}
		if (node->right) {
			if (!is_red(node->right) && !is_red(node->right->left)) {
				node = tree_internal_red_to_right(node);
			}
			if (!tree_key_comparator(key, node->key)) {
				tmp = tree_get_min(node->right);
				node->key = tmp->key;
				node->value = tmp->value;
				node->right = tree_remove_min(node->right);
			} else {
				node->right = tree_node_remove(node->right, key);
			}
		}
	}
	return (tree_fixup(node));
}

struct LLRBNode *tree_node_remove(struct LLRBNode *node, t_key key) {
	node = tree_internal_remove(node, key);
	if (node) {
		node->color = black;
	}
	return (node);
}

int tree_internal_save(struct LLRBNode* node, FILE *fp, int counter, int parent_index) {
	print_debug("node address: %p\n", (void*)node);
	counter = counter + 1;
	int my_index = counter;
	if (NULL == fp) {
		msg_error("fp is null in btree_internal_write");
		exit(1);
	}

	if (node->color == red) {
		fprintf(fp, "\ta%03d [shape=record, label=\"%d\"];\n", counter, node->key);
	} else {
		fprintf(fp, "\ta%03d [shape=record, label=\"%d\"];\n", counter, node->key);
	}

	if (parent_index != -1) {
		// link to parent is red
		if (node->color == red) {
			fprintf(fp, "\ta%03d -> a%03d [color=red];\n", parent_index, counter);
		} else {
			fprintf(fp, "\ta%03d -> a%03d [color=grey];\n", parent_index, counter);
		}
	}

	if (node->left != NULL) {
		counter = tree_internal_save(node->left, fp, counter, my_index);
	}
	if (node->right != NULL) {
		counter = tree_internal_save(node->right, fp, counter, my_index);
	}
	return counter;
}

void tree_save(struct LLRBNode *root, int index) {
	FILE *fp;
	if (index > 1000) {
		msg_warn("index is greater than 1000, tweak btree_save, otherwise output will be a mess");
	}
	char* filename = malloc(20);
	sprintf(filename, "save%d.dot", index);
	if (NULL != filename) {
		fp = fopen(filename, "w");
	} else {
		msg_error("Filename is NULL somehow...");
		exit(1);
	}
	free(filename);
	fprintf(fp, "digraph {\n");
	tree_internal_save(root, fp, 0, -1);
	fprintf(fp, "}");
	fclose(fp);
}



void tree_free(struct LLRBNode *node) {
	if (node) {
		if (node->left)
			tree_free(node->left);
		if (node->right)
			tree_free(node->right);
		node->left = (struct LLRBNode *)NULL;
		node->right = (struct LLRBNode *)NULL;
		free(node);
	}
}
