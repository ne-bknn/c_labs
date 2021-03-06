#ifndef MY_STRUCTURES_H
#define MY_STRUCTURES_H

#include <stddef.h>
#include <stdint.h>

// Tables of linear congrueentail generators of different sizes
// and good lattice structure, Pierre L'Ecuyer
#define HASHTABLE_SPACE_SIZE 65521
#define ROLL 17364

struct Item {
	uint8_t is_set;
	uint8_t skipped;
	char* key;
	struct UnorderedVector* v;
};
struct Hashtable {
	size_t space_size;
	size_t filled;
	struct Item* space;
};

struct Hashtable* hashtable_create();
uint8_t hashtable_insert(struct Hashtable* table, char *key);
struct UnorderedVector* hashtable_get(struct Hashtable* table, char *key);
struct Item* hashtable_find(struct Hashtable* table, char *key);
uint8_t hashtable_delete_item(struct Hashtable* table, char *key);
void hashtable_free(struct Hashtable* table);

// UnorderedVector
struct UnorderedVector {
	size_t size;
	size_t length;
	char** space;
};

struct UnorderedVector* vector_create();
void vector_push(struct UnorderedVector* vector, char* data);
uint8_t vector_delete(struct UnorderedVector* vector, size_t index);
int64_t vector_find(struct UnorderedVector* vector, char* data);
void vector_print(struct UnorderedVector* vector);
void vector_free(struct UnorderedVector* vector);

// Graph
struct Graph {
	struct Hashtable* adj_list;
	struct UnorderedVector* vertex_list;
};

struct Graph* graph_create();
uint8_t graph_add_vertex(struct Graph* graph, char* vertex_name);
uint8_t graph_add_edge(struct Graph* graph, char* vertex_name_1, char* vertex_name_2);
void graph_free(struct Graph *graph);
uint8_t graph_delete_edge(struct Graph* graph, char* vertex_name_1, char* vertex_name_2);
uint8_t graph_delete_vertex(struct Graph* graph, char* vertex_name);
struct UnorderedVector* graph_path(struct Graph* graph, char* vertex_name_1, char* vertex_name_2);
struct Graph* graph_generate();
struct Graph* graph_load(char *filename);
uint8_t graph_output(struct Graph *graph, char *filename);
struct UnorderedVector* graph_path(struct Graph* graph, char* vertex_name_1, char* vertex_name_2);
struct Hashtable* graph_do_bfs(struct Graph *graph, char *start);

// Queue
struct Queue {
        struct QueueNode* front;
        struct QueueNode* rear;
};

struct QueueNode {
	char *value;
	struct QueueNode* next;
};

int queue_is_empty(struct Queue *q);
void queue_enqueue(struct Queue *q, char* value);
char* queue_dequeue(struct Queue *q);
struct Queue* queue_create();
void queue_free(struct Queue *q);
#endif
