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


// TrieNode interface
struct TrieNode {
	char symbol;
	bool is_leaf;
	// a pointer to an array of pointers
	// which point to malloc'd arrays of size 128
	struct TrieNode **children;
};

#endif
