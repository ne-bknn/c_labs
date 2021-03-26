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
uint8_t hashtable_insert(struct Hashtable* table, char *key) {
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
	if (vector->size == vector->length) {
		vector->space = realloc(vector->space, vector->size*2);
		if (NULL == vector->space) {
			msg_error("Failed to realloc vector space!");
			exit(1);
		}
		vector->size = vector->size*2;
	}
	vector->space[vector->length] = data;
}

void vector_delete(struct UnorderedVector* vector, size_t index) {
	if (index+1 > vector->length) {
		msg_error("Out of bounds!");
		exit(1);
	}
	free_z(vector->space[index]);
	vector->length = vector->length - 1;
	if (index+1 != vector->length) {
		vector->space[index] = vector->space[vector->length];
	}

	if (vector->length < vector->size/2 && vector->size != 8) {
		vector->space = realloc(vector->space, vector->size/2);
		vector->size = vector->size / 2;
	}
}
