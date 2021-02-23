#include "lib.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Vector interface

struct Vector* create_vector() {
	struct Vector* pvec = calloc(1, sizeof(struct Vector));
	pvec->filled = 0;
	pvec->max_size = 10;
	pvec->strings = calloc(10, sizeof(char*));
	return pvec;
}

int add_word_to_vector(struct Vector *p_vector, char *str) {
	if (p_vector->filled == p_vector->max_size) {
		size_t new_size = p_vector->max_size * 2;
		void* new_pointers = realloc(p_vector->strings, sizeof(char*)*new_size);
		if (NULL == new_pointers) {
			return 0;
		} else {
			p_vector->max_size = new_size;
			p_vector->strings = (char **)new_pointers;
		}
	}
	p_vector->strings[p_vector->filled] = str;
	++p_vector->filled;
	return 1;

}

void print_vector(struct Vector *p_vector) {
	printf("[ ");
	for (size_t i = 0; i < p_vector->filled; ++i) {
		printf("%s ", p_vector->strings[i]);
	}
	printf("]\n");
}

void free_vector(struct Vector *p_vector) {
	for (size_t i = 0; i < p_vector->filled; ++i) {
		free(p_vector->strings[i]);
	}
	free(p_vector->strings);
	free(p_vector);
}

// Trie interface
struct TrieNode* make_trie_node(char symbol) {
	struct TrieNode *pnode = calloc(1, sizeof(struct TrieNode));
	if (NULL == pnode) {
		return NULL;
	}
	pnode->symbol = symbol;
	pnode->children = calloc(128, sizeof(struct TrieNode*));
	if (NULL == pnode->children) {
		free(pnode);
		return NULL;
	}
	return pnode;
}

int insert_word_in_trie(struct TrieNode* p_root_node, char *word) {
	struct TrieNode *current_node = p_root_node;
	size_t word_len = strlen(word);
	for (size_t i = 0; i < word_len; ++i) {
		size_t current_symbol = (size_t)word[i];
		if (current_symbol < 0 || current_symbol > 127) {
			return 0;
		}
		if (NULL == current_node->children[current_symbol]) {
			struct TrieNode* new_trie_node = make_trie_node(word[i]);
			current_node->children[current_symbol] = new_trie_node;
		}
		current_node = current_node->children[current_symbol];
	}
	current_node->is_leaf = 1;
	return 1;
}

bool is_word_in_trie(struct TrieNode* p_root_node, char *word) {
	struct TrieNode *current_node = p_root_node;
	size_t word_len = strlen(word);
	for (size_t i = 0; i < word_len; ++i) {
		size_t current_symbol = (size_t)word[i];
		if (current_symbol < 0 || current_symbol > 127) {
			return false;
		}
		if (NULL == current_node->children[current_symbol]) {
			return false;
		} else {
			current_node = current_node->children[current_symbol];
		}
	}
	if (NULL != current_node && current_node->is_leaf == 1) {
		return true;
	}
	return false;
}

void free_trie(struct TrieNode* p_root_node) {
	for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
		if (NULL != p_root_node->children[i]) {
			free_trie(p_root_node->children[i]);
		}
	}
	free(p_root_node->children);
	free(p_root_node);
}

// Misc
// reads until newline, adds words to vector 
// of strings
// returns 1 on newline, 2 on eof, 0 on error

int read_line(struct Vector* p_vector) {
	int status1, status2;
	status1 = 1;
	while (status1 > 0) {
		char *word;
		int is_newline = 0;
		status1 = scanf(" %ms", &word);
		status2 = scanf("%*1[\n]%n", &is_newline);

		if (status1 == 1) {
			add_word_to_vector(p_vector, word);
		} else if (status1 == 0) {
			free(word);
			fprintf(stderr, "No matching chars!\n");
		}

		if (is_newline != 0) {
			return 1;
		}
	}

	if (status1 == EOF) {
		print_debug("%s", "EXITING AT EOF");
		return 2;
	}
	// caught something strange
	print_debug("%s", "EXITING AT WTF");
	return 0;
}

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s\n", msg);
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

