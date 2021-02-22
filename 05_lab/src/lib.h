#ifndef LIB_H
#define LIB_H

#include <stddef.h>
#include <stdbool.h>

#define ALPHABET_SIZE 128
#define MAX_STRING_SIZE 256

// Vector of strings interface
struct Vector {
	size_t max_size;
	size_t filled;
	char **strings;
};
struct Vector* create_vector();
int add_word_to_vector(struct Vector *p_vector, char *str);
void print_vector(struct Vector *p_vector);

// TrieNode interface
struct TrieNode {
	char symbol;
	bool is_leaf;
	// a pointer to an array of pointers
	// which point to malloc'd arrays of size 128
	struct TrieNode **children;
};


int read_line(struct Vector *p_vector);
#endif
