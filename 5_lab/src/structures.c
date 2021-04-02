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
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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
		if (current_item->is_set == 0 && current_item->skipped == 0) {
			return NULL;
		}
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


void hashtable_free(struct Hashtable *table) {
	print_debug("%s", "Entered hashtable_free");
	for (size_t i = 0; i < HASHTABLE_SPACE_SIZE; ++i) {
		if (table->space[i].is_set != 0) {
			print_debug("%s", "Found entry to delete");
			print_debug("In hashtable_free. Freeing key: %p", table->space[i].key);
			free_z(table->space[i].key);
			print_debug("%s", "Freeing this vector:");
			vector_free(table->space[i].v);
		}
	}
	free_z(table->space);
	free_z(table);
	print_debug("%s", "Exited hashtable_free");
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
	if (vector->size <= vector->length) {
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
	char** space = vector->space;
	for (size_t i = 0; i < vector->length; ++i) {
		if (strcmp(space[i], data) == 0) {
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
		print_debug("vector_free: freeing pointer %p", vector->space[i]);
		free_z(vector->space[i]);
		print_debug("freed %p", vector->space[i]);
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
	struct Graph* graph = mknew(struct Graph);
	graph->adj_list = hashtable_create();
	graph->vertex_list = vector_create();
	return graph;
}

// returns 1 on general hashtable error, 0 on succ
// 2 on hashtable same key insertion
uint8_t graph_add_vertex(struct Graph* graph, char* vertex_name) {
	uint8_t hashtable_insert_status = hashtable_insert(graph->adj_list, vertex_name);	
	if (hashtable_insert_status == 2) {
		msg_error("Same key insertion attempt!");
		return 2;
	}
	if (hashtable_insert_status == 1) {
		msg_error("The table is full!");
		return 1;
	}
	if (hashtable_insert_status == 3) {
		msg_error("Very weird hashtable error");
		return 1;
	}
	if (hashtable_insert_status != 0) {
		msg_error("Junk return status from hashtable_insert!");
		return 1;
	}
	char* vertex_name_copy = malloc((strlen(vertex_name)+1)*sizeof(char));
	strcpy(vertex_name_copy, vertex_name);
	vector_push(graph->vertex_list, vertex_name_copy);
	return 0;
}

// returns 0 on succ, returns 1 on nonexistent vertices, 2 on edge exists, 3 on weird errors
uint8_t graph_add_edge(struct Graph* graph, char* vertex_name_1, char* vertex_name_2) {
	if (vector_find(graph->vertex_list, vertex_name_1) == -1) {
		msg_warn("First provided vertex does not exist");
		return 1;
	}
	if (vector_find(graph->vertex_list, vertex_name_2) == -1) {
		print_debug("graph_add_edge, vertex_name_2: %s", vertex_name_2);
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
	vector_push(first_vector, vertex_name_2);
	if (strcmp(vertex_name_1, vertex_name_2) != 0) {
		vector_push(second_vector, vertex_name_1);
	}
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
	print_debug("%s", "Entered graph_path");

	if ((vector_find(graph->vertex_list, vertex_name_1) == -1) || (vector_find(graph->vertex_list, vertex_name_2) == -1)) {
		return NULL;
	}

	struct Hashtable* trace = graph_do_bfs(graph, vertex_name_1);
	if (NULL == trace) {
		return NULL;
	}
	print_debug("%s", "created trace");
	if (NULL == hashtable_find(trace, vertex_name_2)) {
		hashtable_free(trace);
		return NULL;
	}
	struct UnorderedVector* path = vector_create();
	vector_push(path, vertex_name_2);
	char* next = strnew(hashtable_get(trace, vertex_name_2)->space[0]);
	while (strcmp(next, vertex_name_1) != 0) {
		vector_push(path, next);
		next = strnew(hashtable_get(trace, next)->space[0]);
	}
	vector_push(path, next);
	hashtable_free(trace);
	return path;
}

// graph generation utils

// returns random string of length 4
char* get_rnd_string(uint8_t length) {
	char* str = malloc(sizeof(char)*(length+1));
	char array[100] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
	for (size_t i = 0; i < length; ++i) {
		str[i] = array[rand()%52];
	}
	str[length] = '\0';
	return str;
}

// for decision making
int get_rnd_bool(uint8_t prob) {
	if (prob >= 100) {
		return 1;
	}
	if ((rand() % 100) > (100-prob)) {
		return 1;
	} else {
		return 0;
	}
}

// for index generation
int get_rnd_int(int lower_than) {
	if (lower_than == 0) {
		return 0;
	}
	return rand() % lower_than;
}

struct Graph* graph_generate() {
	struct Graph* graph = graph_create();
	if (NULL == graph) {
		return NULL;
	}
	
	char *base_name = malloc(sizeof(char)*5);
	strcpy(base_name, "base");
	graph_add_vertex(graph, base_name);

	for (size_t i = 0; i < 20; ++i) {
		char* vertex_name = get_rnd_string(4);
		int status = graph_add_vertex(graph, vertex_name);
		if (status == 2) {
			free_z(vertex_name);
			continue;
		}

		
		int counter = 0;
		while (1) {
			char* vertex_name_copy = strnew(vertex_name);
			int index = get_rnd_int(graph->vertex_list->length);
			char* adj_vertex = graph->vertex_list->space[index];
			char* adj_vertex_copy = strnew(adj_vertex);
			status = graph_add_edge(graph, vertex_name_copy, adj_vertex_copy);
			if (status == 2) {
				free_z(adj_vertex_copy);
				free_z(vertex_name_copy);
				counter += 1;
				if (counter > 10) {
					msg_error("Could not insert new random edge in 10 steps! Graph may be disjoint");
					break;
				}
			} else {
				break;
			}
		}
	}
	
	for (size_t i = 0; i < get_rnd_int(15); ++i) {
		int first_index = get_rnd_int(20);
		int second_index = get_rnd_int(20);
		char* first_vertex_name = graph->vertex_list->space[first_index];
		char* second_vertex_name = graph->vertex_list->space[second_index];

		char* first_vertex_name_copy = strnew(first_vertex_name);
		char* second_vertex_name_copy = strnew(second_vertex_name);
		int status = graph_add_edge(graph, first_vertex_name_copy, second_vertex_name_copy);
		if (status == 2) {
			free_z(first_vertex_name_copy);
			free_z(second_vertex_name_copy);
		}
	}

	int index = get_rnd_int(20);
	char* vertex_name = malloc(sizeof(char)*6);
	char* name_copy = strnew(graph->vertex_list->space[index]);
	
	strcpy(vertex_name, "qwert");
	char* vertex_copy = strnew(vertex_name);
	graph_add_vertex(graph, vertex_copy);
	graph_add_edge(graph, name_copy, vertex_name);

	return graph;
}

struct Graph* graph_load(char *filename) {
	FILE *fp = fopen(filename, "r");
	if (NULL == fp) {
		return NULL;
	}
	
	struct Graph* graph = graph_create();

	char* current_key;
	char* temp_key;
	size_t n_adjacent;
	int status;
	// wrong input; double free somewhere inside
	// wrong input leads to going into "error" ifs
	// and "error" ifs trigger double free for some reason
	while (1) {
		status = fscanf(fp, "%ms", &current_key);
		print_debug("current_key: %s", current_key);
		if (status == -1) {
			print_debug("%s", "Hit EOF, exiting");
			break;
		}
		
		graph_add_vertex(graph, current_key);

		status = fscanf(fp, "%lu", &n_adjacent);
		print_debug("n_adjacent: %lu", n_adjacent);
		if (status != 1) {
			graph_free(graph);
			fclose(fp);
			return NULL;
		}

		for (size_t i = 0; i < n_adjacent; ++i) {
			status = fscanf(fp, "%ms", &temp_key);
			print_debug("%s", "before temp_key output");
			print_debug("temp_key: %s", temp_key);
			print_debug("%s", "after temp_key output");
			if (status != 1) {
				graph_free(graph);
				fclose(fp);
				return NULL;
			}
			status = graph_add_vertex(graph, temp_key);
		// deeper logic needed here
			char* current_key_copy = strnew(current_key);
			print_debug("graph_load, current_key, temp_key: %p, %p", current_key, temp_key);
			print_debug("graph_load, current_key_copy: %s, %p", current_key_copy, current_key_copy);
			char* temp_key_copy = strnew(temp_key);
			if (status == 2) {
				// value is not inserted and will be lost 
				free_z(temp_key);
			}
			print_debug("graph_load, temp_key_copy: %s, %p", temp_key_copy, temp_key_copy);
			status = graph_add_edge(graph, current_key_copy, temp_key_copy);
			if (status == 2) {
				// keys are not inserted and will be lost
				free_z(current_key_copy);
				free_z(temp_key_copy);
			}
		}
	}
	print_debug("%s", "closing fp in graph_load");
	fclose(fp);
	return graph;	
}

uint8_t graph_output(struct Graph *graph, char *filename) {
	FILE *fp;
	if (filename != NULL) {
		fp = fopen(filename, "w");
	} else {
		fp = stdout;
	}
	if (NULL == fp) {
		return 1;
	}

	// key n_of_adjacent_vertecies vertex1 vertex2 vertex3	
	for (size_t i = 0; i < graph->vertex_list->length; ++i) {
		char* current_key = graph->vertex_list->space[i];
		struct UnorderedVector* v = hashtable_get(graph->adj_list, current_key);
		fprintf(fp, "%s ", current_key);
		fprintf(fp, "%lu ", v->length);
		for (size_t j = 0; j < v->length; ++j) {
			fprintf(fp, "%s ", v->space[j]);
		}
		fprintf(fp, "\n");
	}
	if (fp != stdout) {	
		fclose(fp);
	}
	return 0;
}

void graph_free(struct Graph *graph) {
	print_debug("Entered graph_free with pointer: %p", (void*)graph);
	vector_free(graph->vertex_list);
	hashtable_free(graph->adj_list);
	free(graph);
}

// Queue data structure for BFS
// Partially stolen from https://gist.github.com/jitsceait/d4464143f885d0530eda
// Luchshe bi sam napisal kak zhe ploho...

int queue_is_empty(struct Queue *q) {
        if (q->front == NULL && q->rear == NULL) {
		return 1;
	}
        return 0;
}

void queue_enqueue(struct Queue *q, char* value) {
        struct QueueNode *temp = mknew(struct QueueNode);
        temp->value = strnew(value);
        if (queue_is_empty(q)) {
                q->front = temp;
                q->rear = temp;
        } else {
                if (q->rear) {
                        q->rear->next = temp;
                        q->rear = temp;
                } else {
			msg_error("Error in queue occured (q->rear is NULL)");
			free_z(temp->value);
			free_z(temp);
                }
        }
}

char* queue_dequeue(struct Queue *q) {
        if (!queue_is_empty(q)) {
                char* temp = q->front->value;
                struct QueueNode *curr = q->front;
                if (q->front == q->rear) {
                        q->rear = NULL;
                        q->front = NULL;
                }
                else {
                        q->front = curr->next;
                }
                free_z(curr);
                return temp;
        }
	return NULL;
}

struct Queue* queue_create() {
        struct Queue *q = mknew(struct Queue);
        q->front = NULL;
        q->rear = NULL;
	return q;
}

void queue_free(struct Queue *q) {
	char* data = queue_dequeue(q);
	while (NULL != data) {
		free_z(data);
		data = queue_dequeue(q);
	}
	free_z(q);
}

struct Hashtable* graph_do_bfs(struct Graph *graph, char* start) {
	struct Hashtable* trace = hashtable_create();
	struct Queue* q = queue_create();
	struct UnorderedVector* visited = vector_create();
	char* start_copy_1 = strnew(start);
	char* start_copy_2 = strnew(start);
	queue_enqueue(q, start_copy_1);
	vector_push(visited, start_copy_2);
	while (!queue_is_empty(q)) {
		char* key = queue_dequeue(q);
		struct UnorderedVector* neighbors = hashtable_get(graph->adj_list, key);
		if (NULL == neighbors) {
			continue;
		}
		for (size_t i = 0; i < neighbors->length; ++i) {
			char* current_neighbor = neighbors->space[i];
			if (vector_find(visited, current_neighbor) == -1) {
				char *current_neighbor_copy_1 = strnew(current_neighbor);
				queue_enqueue(q, current_neighbor_copy_1);
				char *current_neighbor_copy_2 = strnew(current_neighbor);
				vector_push(visited, current_neighbor_copy_2);
				char *current_neighbor_copy_3 = strnew(current_neighbor);
				hashtable_insert(trace, current_neighbor_copy_3);
				char *key_copy = strnew(key);
				struct UnorderedVector* temp = hashtable_get(trace, current_neighbor_copy_3);
				vector_push(temp, key_copy);
			}
		}
		free_z(key);
	}
	queue_free(q);
	vector_free(visited);
	return trace;
}

