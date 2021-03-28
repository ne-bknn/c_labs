/* 
 * Helper data structures to implement
 * actual graph
 *
 * We need 
 * a) Hashtable char* -> void*
 * b) Dynamically sized vector of char*
*/ 

#include <string.h>
#include <stddef.h>
#include <stdint.h>

#include "structures.h"
#include "utils.h"
#include "tuilib.h"

/* Hashtable char* -> struct Vector*
 * Public interface:
 * hashtable_insert(struct Hashtable* table, char *key) - creates entry in hashtable, creates
 * vector to use as an adjacency list
 * hashtable_get(struct Hashtable* table, char *key) -> struct Vector*
 * hashtable_delete(struct Hashtable* table, char *key) - deletes element from table
 * hashtable_free(struct Hashtable* table)
*/

// Polynomial hash to get index from string
uint16_t polyhash(char* key) {
	size_t len = strlen(key);
	uint32_t hash = 0;
	for (size_t i = 0; i < len; ++i) {
		hash = ((hash+key[i])*ROLL % HASHTABLE_SPACE_SIZE);
	}
	return (uint16_t)hash;
}

struct Hashtable* hashtable_create() {
	struct Hashtable* table = mknew(struct Hashtable);
	if (NULL == table) {
		msg_error("Could not allocate memory for table!");
		exit(1);
	}

	table->space_size = HASHTABLE_SPACE_SIZE;
	table->space = calloc(HASHTABLE_SPACE_SIZE, sizeof(struct Item));
	table->filled = 0;
	if (NULL == table->space) {
		msg_error("Could not allocate memory for keyspace!");
		free_z(table);
		exit(1);
	}
	return table;
}

// inserts key in hashtable, allocates struct Vector*
// returns 2 on same key, tries to insert on hash+1 on collision
// 0 on success, 1 on full table
uint8_t hashtable_insert(struct Hashtable* table, char *key) {
	if (table->space_size == table->filled) {
		return 1;
	}

	uint16_t hash = polyhash(key);
	struct Item* current_item = &table->space[hash];

	if (current_item->is_set) {
		if (strcmp(current_item->key, key) == 0) {
			return 2;
		}
		current_item->skipped = 1;

	}
	return 0;	
}


// UnorderedVector of char* 
// Plain dynamically sized array of pointers to char
// Pushes to the end, on deletion frees the pointer
// and moves pointer at the end on freed space
// so insertion and deletion are O(1), but
// there is no order (we do not need it anyway)

struct UnorderedVector* vector_create() {
	struct UnorderedVector* vector = mknew(struct UnorderedVector);
	if (NULL == vector) {
		msg_error("Could not allocate memory for UVector!");
		exit(1);
	}
	vector->size = 8;
	vector->length = 0;
	vector->space = calloc(vector->size, sizeof(char*));
	if (NULL == vector->space) {
		msg_error("Could not allocate memory for UVector space!");
		free_z(vector);
		exit(1);
	}
	return vector;
}

void vector_push(struct UnorderedVector* vector, char* data) {
	print_debug("Should i realloc? %u", vector->size <= vector->length);
	if (vector->size <= vector->length) {
		print_debug("%s", "reallocing");
		vector->space = realloc(vector->space, sizeof(char*)*(vector->size*2));
		if (NULL == vector->space) {
			msg_error("Failed to realloc vector space!");
			exit(1);
		}
		vector->size = vector->size*2;
	}
	vector->space[vector->length] = data;
	vector->length += 1;
}

// returns 0 on success, 1 on out of bounds
uint8_t vector_delete(struct UnorderedVector* vector, size_t index) {
	// at length 0 any unsigned index is invalid
	// at legnth 1 index 0 is valid, greater is invalid
	// at length n index <=n-1 is valid, greater is invalid
	if (index >= vector->length) {
		print_debug("%s", "Out of bounds");
		return 1;
	}

	free_z(vector->space[index]);
	vector->length = vector->length - 1;
	// now moving condition - if we popped from the end
	// we do not need to move anythin
	// if 
	if (index != vector->length) {
		print_debug("%s", "Freed rightmost element");
		vector->space[index] = vector->space[vector->length];
	}
	
	// we want to realloc the array we do not want to make it shorter than 8
	if (vector->length < vector->size/2 && vector->size != 8) {
		vector->space = realloc(vector->space, sizeof(char*)*(vector->size/2));
		if (NULL == vector->space) {
			msg_error("Failed to realloc vector space!");
			exit(1);
		}
		vector->size = vector->size / 2;
	}
	return 0;
}

void vector_free(struct UnorderedVector* vector) {
	for (size_t i = 0; i < vector->length; ++i) {
		free_z(vector->space[i]);
	}
	free_z(vector->space);
	free_z(vector);
}

void vector_print(struct UnorderedVector *vector) {
	printf("S: %lu, L: %lu [", vector->size, vector->length);
	for (size_t i = 0; i < vector->length; ++i) {
		printf("\"%s\", ", vector->space[i]);
	}
	printf("]\n");
}
