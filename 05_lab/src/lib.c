#include "lib.h"
#include <string.h>
#include <stdio.h>
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
	printf("Vector: \n");
	for (size_t i = 0; i < p_vector->filled; ++i) {
		printf("%s ", p_vector->strings[i]);
	}
	printf("\n");
}

// Trie interface
//struct TrieNode* make_trie_node(char symbol) {
//	struct TrieNode *node = calloc(1, sizeof(struct TrieNode));
//	return node;
//}

// Misc

// reads until newline, adds words to vector 
// of strings
// returns 1 on newline, 2 on eof, 0 on error
int read_line(struct Vector* p_vector) {
	char *word = calloc(10, sizeof(char));
	int status1, status2;
	status1 = 1;
	while (status1 > 0) {
		int is_newline = 0;
		status1 = scanf("%ms", &word);
		status2 = scanf("%*[\n]%n", &is_newline);
		if (status1 == 1) {
			add_word_to_vector(p_vector, word);
		} else if (status1 != 0) {
			perror("scanf");
		} else {
			fprintf(stderr, "No matching chars!\n");
		}
		// caught newline
		if (is_newline == 1) {
			return 1;
		}
	}
	// caught eof
	if (status1 == EOF) {

		return 2;
	}
	// caught something strange
	return 0;
}

