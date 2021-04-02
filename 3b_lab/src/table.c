#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "tuilib.h"
#include "utils.h"
#include "table.h"

// 1 - success, 2 - no filename assigned, 3 - file opening error, 4 - error writing to file
uint8_t table_save(struct Table* table) {
	print_debug("%s", "In table save");
	// overwrites only header - two uint64_t and two spaces of Items
	if (NULL == table->filename) {
		return 2;
	}
	FILE *fp;
	fp = fopen(table->filename, "r+b");
	if (NULL == fp) {
		return 3;
	}
	fseek(fp, 0, SEEK_SET);
	if (fwrite(&table->space_size, sizeof(uint64_t), 1, fp) != 1) {
		return 4;
	}
	if (fwrite(&table->data_disk_size, sizeof(uint64_t), 1, fp) != 1) {
		return 4;
	}
	if (fwrite(table->space1, sizeof(struct Item), table->space_size, fp) != table->space_size) {
		return 4;
	}
	if (fwrite(table->space2, sizeof(struct Item), table->space_size, fp) != table->space_size) {
		return 4;
	}
	print_debug("%s", "closed file");
	fclose(fp);
	return 0;
}

struct Table* table_create(char* filename) {
	struct Table* table = malloc(sizeof(struct Table));
	if (NULL == table) {
		msg_error("Could not allocate memory for table!");
		exit(1);
	}
	
	// a prime close to 2**16 - max space because keys are uint16_t
	table->space_size = SPACE_SIZE;
	table->data_disk_size = 0;
	table->filename = filename;

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

	if (access(filename, F_OK) == 0) {
		// file exists
		print_debug("%s", "Such file exists, reading table from it");
		FILE *fp = fopen(filename, "r+b");
		fseek(fp, 0, SEEK_SET);
		fread(&table->space_size, sizeof(uint64_t), 1, fp);
		fread(&table->data_disk_size, sizeof(uint64_t), 1, fp);
		fread(table->space1, sizeof(struct Item), table->space_size, fp);
		fread(table->space2, sizeof(struct Item), table->space_size, fp);
		fclose(fp);
	} else {
		print_debug("%s", "No such file exists, writing");
		FILE *fp = fopen(table->filename, "w");
		fclose(fp);
		table_save(table);
	}
	
	return table;
}

void table_delete(struct Table* table) {
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
// FINDS AN INDEX OF SPACE TO INSERT!!!
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
	item1->is_deleted = 0;
	item1->key1 = key1;
	item1->key2 = key2;
	item1->info_length = strlen(data);
	item1->index1 = index1;
	item1->index2 = index2;

	// filling second item
	struct Item *item2 = &(table->space2[index2]);
	item2->is_set = 255;
	item2->is_deleted = 0;
	item2->key1 = key1;
	item2->key2 = key2;
	item2->info_length = strlen(data);
	item2->index1 = index1;
	item2->index2 = index2;
	
	// writing to file routine
	FILE *fp = fopen(table->filename, "r+b");
	fseek(fp, sizeof(uint64_t)*2, SEEK_SET);
	fseek(fp, sizeof(struct Item)*SPACE_SIZE*2, SEEK_CUR);
	fseek(fp, sizeof(char)*table->data_disk_size, SEEK_CUR);
	if (fwrite(data, sizeof(char), item1->info_length, fp) != item1->info_length) {
		return 4;
	}

	item2->offset = table->data_disk_size;
	item1->offset = table->data_disk_size;

	table->data_disk_size += item1->info_length;
	table_save(table);
	fclose(fp);

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
		
		table->space1[second_index].is_set = 0;
		table->space1[second_index].is_deleted = 1;

		table->space2[index].is_set = 0;
		table->space2[index].is_deleted = 1;
	} else {
		msg_error("Got wrong keyspace in inner function, this should not happen!");
		exit(1);
	}
	table_save(table);
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

void item_print(struct Table *table, struct Item* item) {
	char* temp = malloc(sizeof(char)*(item->info_length+1));
	FILE *fp = fopen(table->filename, "r+b");
	print_debug("Data offset: %lu", item->offset+sizeof(struct Item)*SPACE_SIZE*2+sizeof(uint64_t)*2);
	fseek(fp, item->offset, SEEK_SET);
	fseek(fp, SPACE_SIZE*2*sizeof(struct Item)+sizeof(uint64_t)*2, SEEK_CUR);
	fread(temp, sizeof(char), item->info_length, fp);
	temp[item->info_length] = '\0';

	print_debug("%s", "Before print");	
	printf("Key #1: %"PRIu32" Key #2: %"PRIu32" Index #1: %"PRIu16" Index #2: %"PRIu16" Data: %s\n", item->key1, item->key2, item->index1, item->index2, temp);
	print_debug("%s", "After print");	
	free(temp);
}


