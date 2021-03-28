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
	struct Vector* v;
};

struct Hashtable {
	size_t space_size;
	size_t filled;
	struct Item* space;
};

struct Hashtable* hashtable_create();

// UnorderedVector
struct UnorderedVector {
	size_t size;
	size_t length;
	char** space;
};

struct UnorderedVector* vector_create();
void vector_push(struct UnorderedVector* vector, char* data);
uint8_t vector_delete(struct UnorderedVector* vector, size_t index);
void vector_print(struct UnorderedVector* vector);
void vector_free(struct UnorderedVector* vector);

#endif
