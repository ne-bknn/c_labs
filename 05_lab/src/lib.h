#ifndef LIB_H
#define LIB_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define ALPHABET_SIZE 128
#define MAX_STRING_SIZE 256

#define DEBUG 1
#define print_debug(fmt, ...) \
	do { if (DEBUG) { fprintf(stderr, "\n%s", "[.] "); fprintf(stderr, fmt, __VA_ARGS__); fprintf(stderr, "%s", "\n"); } } while (0)

// Vector of strings interface
struct Vector {
	size_t max_size;
	size_t filled;
	char **strings;
};
struct Vector* create_vector();
int add_word_to_vector(struct Vector *p_vector, char *str);
void print_vector(struct Vector *p_vector);
void free_vector(struct Vector *p_vector);
// TrieNode interface
struct TrieNode {
	char symbol;
	bool is_leaf;
	// a pointer to an array of pointers
	// which point to malloc'd arrays of size 128
	struct TrieNode **children;
};
struct TrieNode* make_trie_node(char symbol);
int insert_word_in_trie(struct TrieNode* p_root_node, char *word);
bool is_word_in_trie(struct TrieNode* p_root_node, char *word);
void free_trie(struct TrieNode* p_root_node);

// Misc
int read_line(struct Vector *p_vector);

void print_warning(char msg[]);

void print_error(char msg[]);


#endif
