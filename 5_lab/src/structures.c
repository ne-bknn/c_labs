/* 
 * Helper data structures to implement
 * actual graph
 *
 * We need 
 * a) Hashtable char* -> struct UnorderedVector*
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

// just a convenience function
struct UnorderedVector* hashtable_get(struct Hashtable* table, char *key) {
	struct Item* item = hashtable_find(table, key);
	if (NULL == item) {
		return NULL;
	} else {
		return item->v;
	}
}

struct Item* hashtable_find(struct Hashtable* table, char *key) {
	uint16_t initial_hash = polyhash(key);
	uint16_t current_hash = initial_hash;
	while (1) {
		struct Item* current_item = &table->space[current_hash];
		if (strcmp(current_item->key, key) == 0) {
			return current_item;
		}

		if (current_item->skipped != 1) {
			// current item is not ours
			// and the element has never been skipped
			// so this path was never taken
			return NULL;
		}
		current_hash = (current_hash + 1) % HASHTABLE_SPACE_SIZE;
		if (current_hash == initial_hash) {
			return NULL;
		}
	}

}

// 1 on success, 0 on no such key
uint8_t hashtable_delete_item(struct Hashtable* table, char *key) {
	struct Item* item = hashtable_find(table, key);
	if (NULL == item) {
		return 0;
	}
	free_z(item->key);
	vector_free(item->v);
	item->is_set = 0;
	return 1;
}


// inserts key in hashtable, allocates struct Vector*
// returns 2 on same key, tries to insert on hash+1 on collision
// 0 on success, 1 on full table, 3 on very weird error
uint8_t hashtable_insert(struct Hashtable* table, char *key) {
	if (table->space_size == table->filled) {
		return 1;
	}

	uint16_t initial_hash = polyhash(key);
	uint16_t current_hash = initial_hash;
	struct Item* current_item = &table->space[initial_hash];
	struct Item* first_item = current_item;
	
	while (table->space[current_hash].is_set) {
		if (strcmp(current_item->key, key) == 0) {
			return 2;
		}
		current_hash++;
		current_item = &table->space[current_hash];
		if (current_hash == initial_hash) {
			// we returned back: this should NOT happen...
			msg_warn("Returned back in insert, this should NOT happen");
			return 3;
		}
	}
	
	// splitting it in two for atomicity: there wont be any
	// additional skipped marks in case of error
	while (first_item != current_item) {
		first_item->skipped = 1;
		first_item++;
	}
	current_item->key = key;
	current_item->is_set = 1;
	current_item->v = vector_create();
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

int64_t vector_find(struct UnorderedVector* vector, char* data) {
	char* iter = *vector->space;
	for (int64_t i = 0; i < vector->length; ++i) {
		if (strcmp(iter+i, data) == 0) {
			return i;
		}
	}
	return -1;
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

// Graph interface
struct Graph* graph_create() {
	struct Hashtable* table = hashtable_create();
	struct Graph* graph = mknew(struct Graph);
	graph->adj_list = table;
	graph->vertex_list = vector_create();
	return graph;
}

uint8_t graph_add_vertex(struct Graph* graph, char* vertex_name) {
	uint8_t hashtable_insert_status = hashtable_insert(graph->adj_list, vertex_name);	
	if (hashtable_insert_status != 0) {
		msg_error("Something went wrong when trying to insert in hashtable");
		return 1;
	}
	vector_push(graph->vertex_list, vertex_name);
	return 0;
}

// returns 0 on succ, returns 1 on nonexistent vertices, 2 on edge exists, 3 on weird errors
uint8_t graph_add_edge(struct Graph* graph, char* vertex_name_1, char* vertex_name_2) {
	if (vector_find(graph->vertex_list, vertex_name_1) == -1) {
		msg_warn("First provided vertex does not exist");
		return 1;
	}
	if (vector_find(graph->vertex_list, vertex_name_2) == -1) {
		msg_warn("Second provided vertex does not exist");
		return 1;
	}
	struct UnorderedVector* first_vector = hashtable_get(graph->adj_list, vertex_name_1);
	if (NULL == first_vector) {
		msg_error("Something weird happened: first vector is NULL");
		return 3;
	}
	struct UnorderedVector* second_vector = hashtable_get(graph->adj_list, vertex_name_2);
	if (NULL == second_vector) {
		msg_error("Something weird happened: second vector is NULL");
		return 3;
	}
	int64_t vertex_index;
	if (first_vector->length <= second_vector->length) {
		vertex_index = vector_find(first_vector, vertex_name_2);
	} else {
		vertex_index = vector_find(second_vector, vertex_name_1);
	}
	if (vertex_index != -1) {
		msg_warn("Such an edge already exists");
		return 2;
	}
	print_debug("%s", "Creating edge");	
	vector_push(first_vector, vertex_name_2);
	vector_push(second_vector, vertex_name_1);
	return 0;
}

uint8_t graph_delete_edge(struct Graph* graph, char* vertex_name_1, char* vertex_name_2) {
	if (vector_find(graph->vertex_list, vertex_name_1) == -1) {
		msg_warn("First provided vertex does not exist");
		return 1;
	}

	if (vector_find(graph->vertex_list, vertex_name_2) == -1) {
		msg_warn("Second provided vertex does not exist");
		return 1;
	}

	struct UnorderedVector* first_vector = hashtable_get(graph->adj_list, vertex_name_1);
	if (NULL == first_vector) {
		msg_error("Something weird happened: first vector is NULL");
		return 3;
	}
	int64_t vertex_index = vector_find(first_vector, vertex_name_2);
	if (vertex_index == -1) {
		msg_warn("Vertecies are not connected");
		return 2;
	}
	struct UnorderedVector* second_vector = hashtable_get(graph->adj_list, vertex_name_2);
	if (NULL == second_vector) {
		msg_error("Something weird happened: second vector is NULL");
		return 3;
	}
	int64_t another_vertex_index = vector_find(second_vector, vertex_name_1);
	if (vertex_index == -1) {
		msg_error("Weird error: vertex1 is connected with vertex2, but not otherwise");
		return 3;
	}
	vector_delete(first_vector, vertex_index);
	vector_delete(second_vector, another_vertex_index);
	return 0;

}

uint8_t graph_delete_vertex(struct Graph* graph, char* vertex_name) {
	if (vector_find(graph->vertex_list, vertex_name) == -1) {
		msg_warn("No such vertex exists");
		return 1;
	}

	struct UnorderedVector* vector = hashtable_get(graph->adj_list, vertex_name);
	for (size_t i = 0; i < vector->length; ++i) {
		char* current_key = vector->space[i];
		struct UnorderedVector* inner_vector = hashtable_get(graph->adj_list, current_key);
		if (NULL == inner_vector) {
			msg_warn("Hit nonexistent key while deleting: this should not happen");
			continue;
		}
		int64_t index = vector_find(inner_vector, vertex_name);
		if (index == -1) {
			msg_warn("Hit no connection with starting vertex in adjacent vertex: this should not happen");
			continue;
		}
		vector_delete(inner_vector, index);
	}
	uint8_t status = hashtable_delete_item(graph->adj_list, vertex_name);
	if (status == 0) {
		msg_error("Weird! No such key after all the work! This should NOT happen!");
		return 2;
	}
	int64_t index = vector_find(graph->vertex_list, vertex_name);
	if (index == -1) {
		msg_error("WEIRD! No key in vertex_list during deletion!");
		return 2;
	}
	vector_delete(graph->vertex_list, index);
	return 0;
}

// struct is called UnorderedVector, but it becomes unordered only because of deletion, so i guess i'll use it here..
struct UnorderedVector* graph_path(struct Graph* graph, char* vertex_name_1, char* vertex_name_2) {
	return NULL;
}

void graph_generate(struct Graph* graph) {
}

void graph_load(struct Graph* graph) {
}

void graph_save(struct Graph* graph) {
}
