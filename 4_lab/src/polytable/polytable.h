#include <stdint.h>
#include <stddef.h>

#ifndef POLYTABLE_H
#define POLYTABLE_H

#define SPACE_SIZE 256

struct Item {
	uint8_t is_set;
	uint8_t key;
	void* data;
};

struct Polytable {
	struct Item* space;
	size_t space_size;
	size_t filled;
};

struct Polytable* polytable_create();
uint8_t polytable_insert(struct Polytable* table, uint8_t key, void* value_ptr);

void* polytable_get(struct Polytable* table, uint8_t key);
void polytable_delete(struct Polytable* table);
void polytable_print(struct Polytable* table);
#endif

#ifndef POLYTABLE_INTERFACE_H
#define POLYTABLE_INTERFACE_H

#include "polytable.h"

void tuilib_polytable_insert(void** args, void* main_structure);
void tuilib_polytable_get(void** args, void* main_structure);
void tuilib_polytable_print(void** args, void* main_structure);

#endif
