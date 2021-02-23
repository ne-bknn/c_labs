#include <stdio.h>
#include "lib.h"
#include <string.h>
#include <stdlib.h>

int main() {
	int status_code = 1;
	while (status_code == 1) {
		struct Vector* p_first_vec = create_vector();
		struct TrieNode* p_root_node = make_trie_node('\0');
		status_code = read_line(p_first_vec);
		if (status_code != 1) {
			print_error("Unexpected EOF!");
			free_trie(p_root_node);
			free_vector(p_first_vec);
			exit(1);
		}
		struct Vector* p_second_vec = create_vector();
		status_code = read_line(p_second_vec);
		if (status_code != 1) {
			free_trie(p_root_node);
			free_vector(p_first_vec);
			free_vector(p_second_vec);
			return 0;
		}
		for (size_t i = 0; i < p_second_vec->filled; ++i) {
			status_code = insert_word_in_trie(p_root_node, p_second_vec->strings[i]);
			if (status_code == 0) {
				print_error("Got unprintable symbol, exiting");
				exit(1);
			}
		}
		struct Vector* p_answer_vec = create_vector();
		for (size_t i = 0; i < p_first_vec->filled; ++i) {
			char *current_word = p_first_vec->strings[i];
			if (is_word_in_trie(p_root_node, current_word)) {
				char *word_copy = calloc(sizeof(char), strlen(current_word)+1);
				strncpy(word_copy, current_word, strlen(current_word));
				add_word_to_vector(p_answer_vec, word_copy);	
			}
		}
		printf("Result vector:\n");
		print_vector(p_answer_vec);
		free_trie(p_root_node);
		free_vector(p_first_vec);
		free_vector(p_second_vec);
		free_vector(p_answer_vec);
	}
	return 0;
}
