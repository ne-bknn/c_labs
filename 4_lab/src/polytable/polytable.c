/*
 * polytable.c: generic int to void* hashtable
 * intended to be used as a cache in front of
 * persistent data structure: values are purged 
 * out of table when table is full.
 *
 * Public interface:
 * polytable_create() - returns table object
 * polytable_insert(table, key, value_ptr) - inserts element in table
 * polytable_get(table, key) - returns value_ptr when element is found, NULL otherwise
 * polytable_delete(table) - deletes the table
 *
 * There's no deletion of specific element, since it is not needed
 *
 * (c) ne_bknn 2021
*/ 

/*
 * TODO: My mental model was a bit off and there's no situation when 
 * key1 != key2, but index1 == index2, because ||key_space|| == ||index_space||
 * This table is ought to be tweaked a bit, rerolling wont be ever used
 * in this realisation
*/

#include <stddef.h>
#include <stdint.h>
#include "polytable.h"
#include <stdio.h>

#include "../tuilib.h"
#include "../utils.h"

struct Polytable* polytable_create() {
	struct Polytable* table = mknew(struct Polytable);
	if (NULL == table) {
		msg_error("Could not allocate memory for cache!");
		exit(1);
	}

	table->space_size = SPACE_SIZE;
	table->space = calloc(SPACE_SIZE, sizeof(struct Item));
	if (NULL == table->space) {
		msg_error("Could not allocate keyspace for cache!");
		free_z(table);
		exit(1);
	}
	
	return table;
}

uint8_t hash(uint8_t key) {
	return (key*37+127);
}


// returns 1 on success, 0 on collision
uint8_t polytable_insert(struct Polytable* table, uint8_t key, void* value_ptr) {
	uint8_t index = hash(key);
	uint8_t initial_index = index;
	for (uint8_t i = 0; i < 5; ++i) {
		if (table->space[index].is_set == 0 || table->space_size == table->filled || table->space[index].key == key) {
			table->space[index].is_set = 1;
			table->space[index].key = key;
			table->space[index].data = value_ptr;
			return 1;
		}

		index = hash(index);
	}	
	// exited loop: there is no empty space during first 4 rehashings
	// inserting at initial_index
	table->space[initial_index].key = key;
	table->space[initial_index].data = value_ptr;
	return 1;
}

void* polytable_get(struct Polytable* table, uint8_t key) {
	uint8_t index = hash(key);
	for (uint8_t i = 0; i < 5; ++i) {
		if (table->space[index].key == key) {
			return table->space[index].data;
		}
		index = hash(index);
	}
	// we didn't find out key among first 5 elements - it wont be here
	return NULL;
}

void polytable_delete(struct Polytable* table) {
	// freeing data in pointers in polytable
	// is not it's responsibility
	// because it's not persistent data storage
	free_z(table->space);
	free_z(table);
}

// for debugging purposes
void polytable_print(struct Polytable* table) {
	for (size_t i = 0; i < SPACE_SIZE; ++i) {
		if (table->space[i].is_set) {
			printf("#%lu. Key: %u. Ptr: %p\n", i, table->space[i].key, table->space[i].data);
		}
	}
}

// interface
void tuilib_polytable_insert(void **args, void *main_structure) {
	struct Polytable *table = (struct Polytable*)main_structure;
	uint8_t *key = (uint8_t*)args[0];
	void *value_ptr = args[1];
	uint8_t status = polytable_insert(table, *key, value_ptr);
	printf("Insert status: %u\n", status);
}

void tuilib_polytable_get(void **args, void *main_structure) {
	struct Polytable *table = (struct Polytable*)main_structure;
	uint8_t *key = (uint8_t*)args[0];
	void* value_ptr = polytable_get(table, *key);
	if (NULL == value_ptr) {
		printf("No such key exists! (Got NULL pointer)");
	} else {
		printf("Returned pointer: %p\n", value_ptr);
	}
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void tuilib_polytable_print(void **args, void *main_structure) {
#pragma clang diagnostic pop
	struct Polytable *table = (struct Polytable*)main_structure;
	polytable_print(table);
}
