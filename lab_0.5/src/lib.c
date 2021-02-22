#include "lib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Vector interface
int add_string_to_vector(struct Vector *p_vector, char* str) {
	return 1;
}

void print_vector(struct Vector *p_vector) {
	printf("Vector: \n");
}

// Trie interface
struct TrieNode* make_trie_node(char symbol) {
	struct TrieNode *node = calloc(1, sizeof(struct TrieNode));
	return node;
}

// Misc


