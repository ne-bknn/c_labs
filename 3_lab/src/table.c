#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "tuilib.h"
#include "utils.h"
#include "table.h"

struct Table* table_create() {
	struct Table* table = malloc(sizeof(struct Table));
	if (NULL == table) {
		msg_error("Could not allocate memory for table!");
		exit(1);
	}
	
	// a prime close to 2**16 - max space because keys are uint16_t
	table->space_size = SPACE_SIZE;

	table->space1 = calloc(sizeof(struct Item), table->space_size);
	if (NULL == table) {
		msg_error("Could not allocate memory for first space!");
		free_z(table);
		exit(1);
	}

	table->space2 = calloc(sizeof(struct Item), table->space_size);
	if (NULL == table) {
		msg_error("Could not allocate memory for second space!");
		free_z(table->space1);
		free_z(table);
		exit(1);
	}

	table->filename = NULL;
	table->autosave = false;
	return table;
}

void table_delete(struct Table* table) {
	for (size_t i = 0; i < table->space_size; ++i) {
		if (table->space1[i].is_set != 0) {
			free_z(table->space1[i].info);
		}
	}
	free_z(table->space1);
	free_z(table->space2);
	free_z(table);
}

// return value is guaranteed to fit inside uint16_t
// but for rehashing purposes we return uint32_t
uint32_t hash1(uint32_t key) {
	// 31991 - prime close to 32000
	// key is 32 bits so no overflow could occur
	return (key + 31991) % SPACE_SIZE;
}

uint32_t hash2(uint32_t key) {
	// another prime close to 32000
	return (key + 32009) % SPACE_SIZE;
}

// pointers to functions are awesome
// lacking pointers to functions with undefined arguments and return type
// void** is ugly
// 1 in status - success
// 0 in status - no space left
// 2 in status - same key
struct Tuple get_index(struct Item* space, uint32_t key, hash func, uint8_t key_space) {
	// first - status code, second - index, if any
	struct Tuple answer = {0, 0};
	uint16_t index = (uint16_t)func(key);
	uint16_t initial_index = index;
	while (true) {
		if (space[index].is_set == 0) {
			// found empty space, hooray!
			answer.first = 1;
			answer.second = index;
			return answer;
		} else {
			// checking for same key
			// we have to pass key space explicitly for this
			if (key_space == 1) {
				if (space[index].key1 == key) {
					answer.first = 2;
					return answer;
				}
			} else if (key_space == 2) {
				if (space[index].key2 == key) {
					answer.first = 2;
					return answer;
				}
			} else {
				msg_error("Got wrong space key, this should not happen!");
				exit(1);
			}

			// starting to search for space, rehashing
			index = func((uint32_t)index);
			if (index == initial_index) {
				// no space left... grustnaya pesnya pro mamu
				return answer;
			}
		}
	}
}

// 0 on out of space, 2 on same key, 1 success
uint8_t table_insert(struct Table* table, uint32_t key1, uint32_t key2, char* data) {
	print_debug("%s", "table_insert");
	// calculating first hash
	struct Tuple first_index = get_index(table->space1, key1, *hash1, 1);	

	if (first_index.first == 0) {
		return 0;
	}

	if (first_index.first == 2) {
		return 2;
	}
	
	uint16_t index1 = first_index.second;

	struct Tuple second_index = get_index(table->space2, key2, *hash2, 2);

	if (second_index.first == 0) {
		return 0;
	}

	if (second_index.first == 2) {
		return 2;
	}
	
	uint16_t index2 = second_index.second;
	print_debug("%s", "table_insert filling data");
	// filling first item
	struct Item *item1 = &(table->space1[index1]);
	item1->is_set = 255;
	item1->is_deleted = false;
	item1->key1 = key1;
	item1->key2 = key2;
	item1->info = data;
	print_debug("%s", "before strlen");
	item1->info_length = strlen(data);
	print_debug("%s", "after strlen");
	item1->offset = 0;
	item1->index1 = index1;
	item1->index2 = index2;
	
	// filling second item
	struct Item *item2 = &(table->space2[index2]);
	item2->is_set = 255;
	item2->is_deleted = false;
	item2->key1 = key1;
	item2->key2 = key2;
	item2->info = data;
	item2->info_length = strlen(data);
	item2->offset = 0;
	item2->index1 = index1;
	item2->index2 = index2;

	return 1;
}

struct Tuple table_find(struct Table *table, uint8_t key_space, uint32_t key) {
	// 0 - no such key, 1 - key found
	// first - status, second - index
	struct Tuple answer = {0, 0};	
	hash func;
	if (key_space == 1) {
		func = hash1;
	} else if (key_space == 2) {
		func = hash2;
	} else {
		msg_error("Got wrong space key, this should not happen!");
		exit(1);
	}

	uint16_t index = (uint16_t)func(key);
	uint16_t initial_index = index;
	while (true) {
		if (key_space == 1) {
			if (table->space1[index].is_set == 0 && !table->space1[index].is_deleted) {
				// not set and not deleted - such key path never existed, exiting
				return answer;
			}
			if (table->space1[index].key1 == key) {
				answer.second = index;
				answer.first = 1;
				return answer;
			} else {
				index = func((uint32_t)index);
				if (index == initial_index) {
					// not found
					return answer;
				}
			}
		} else {
			if (table->space2[index].is_set == 0 && !table->space2[index].is_deleted) {
				// not set and not deleted - such key path never existed, exiting
				return answer;
			}
			if (table->space2[index].key2 == key) {
				answer.second = index;
				answer.first = 1;
				return answer;
			} else {
				index = func((uint32_t)index);
				if (index == initial_index) {
					// not found
					return answer;
				}
			}
	
		}
	}
}

uint8_t item_delete(struct Table* table, uint8_t key_space, uint32_t key) {
	struct Tuple answer;
	if (key_space == 1) {
		print_debug("%s", "In first if");
		answer = table_find(table, key_space, key);
		if (answer.first == 0) {
			print_debug("%s", "returning zero from item_delete");
			return 0;
		}
		uint16_t index = answer.second;
		uint16_t second_index = table->space1[index].index2;
		print_debug("data in item we deleting: %s", table->space1[index].info);
		free_z(table->space1[index].info);
		table->space1[index].is_set = 0;
		table->space1[index].is_deleted = 1;
		table->space2[second_index].is_set = 0;
		table->space2[second_index].is_deleted = 1;
	} else if (key_space == 2) {
		print_debug("%s", "In second if");
		answer = table_find(table, key_space, key);
		if (answer.first == 0) {
			return 0;
		}
		uint16_t index = answer.second;
		uint16_t second_index = table->space1[index].index2;
		print_debug("data in item we deleting: %s", table->space2[index].info);
		free_z(table->space2[index].info);
		
		table->space1[second_index].is_set = 0;
		table->space1[second_index].is_deleted = 1;

		table->space2[index].is_set = 0;
		table->space2[index].is_deleted = 1;
	} else {
		msg_error("Got wrong keyspace in inner function, this should not happen!");
		exit(1);
	}
	return 1;
}

struct Item* table_get(struct Table *table, uint8_t key_space, uint32_t key) {
	struct Tuple index = table_find(table, key_space, key);
	if (index.first != 1) {
		return NULL;
	}
	if (key_space == 1) {
		struct Item* item = &table->space1[index.second];
		return item;
	} else if (key_space == 2) {
		struct Item* item = &table->space2[index.second];
		return item;
	}
	return NULL;
}

void item_print(struct Item* item) {
	printf("Key #1: %"PRIu32" Key #2: %"PRIu32" Index #1: %"PRIu16" Index #2: %"PRIu16" Data: %s\n", item->key1, item->key2, item->index1, item->index2, item->info);
}

void* tuilib_item_delete(void **callback_data, void *main_structure) {
	print_debug("%s", "In tuilib_item_delete");
	struct Table *table = (struct Table*)main_structure;
	uint8_t keyspace = (uint8_t)(*(int*)callback_data[0]);
	if (keyspace != 1 && keyspace != 2) {
		msg_warn("Got wrong keyspace!");
		int *status = malloc(sizeof(int));
		*status = 0;
		return status;
	}
	uint32_t key = (uint32_t)(*(int*)callback_data[1]);
	int *status = malloc(sizeof(int));
	*status = item_delete(table, keyspace, key);
	return status;
}

void* tuilib_item_get(void **callback_data, void *main_structure) {
        struct Table *table = (struct Table*)main_structure;
        print_debug("%s", "Item get");                         
        uint8_t keyspace = (uint8_t)(*(int*)callback_data[0]);
	print_debug("%s", "1234");
	if (keyspace != 1 && keyspace != 2) {
		msg_warn("Got wrong keyspace!");
		return NULL;
	}
        uint32_t key = (uint32_t)(*(int*)callback_data[1]);
	print_debug("%s", "111111");
        struct Item* item = table_get(table, keyspace, key);
	if (NULL == item) {
		msg_warn("No such key exists!");
		return NULL;
	}
	item_print(item);
        return NULL;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void* tuilib_table_print(void **callback_data, void *main_structure) {
#pragma clang diagnostic pop
	struct Table* table = (struct Table*)main_structure;
	for (size_t i = 0; i < table->space_size; ++i) {
		if (table->space1[i].is_set != 0) {
			print_debug("is_set: %d", table->space1[i].is_set);
			item_print(&table->space1[i]);
		}
	}
	return NULL;
}

void* tuilib_table_insert(void **callback_data, void *main_structure) {
        struct Table *table = (struct Table*)main_structure;
        print_debug("%s\n", "Insert");
        int key1 = (uint32_t)(*(int*)callback_data[0]);
        int key2 = (uint32_t)(*(int*)callback_data[1]);
        char *data = (char*)callback_data[2];
	print_debug("data strlen: %lu, allocating: %lu\n", strlen(data), sizeof(char)*(strlen(data)+1));
	char *new_data = malloc(sizeof(char)*(strlen(data)+1));
	// strncpy(new_data, data, strlen(data) fails, why????
	strcpy(new_data, data);
	print_debug("new_data: %s\n", new_data);
	int *status = malloc(sizeof(int));
        *status = table_insert(table, key1, key2, new_data);
	print_debug("Return status in insert: %d\n", *status);
	return (void*)status;
}
