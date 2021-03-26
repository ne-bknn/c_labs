#include <stdint.h>
#include <stddef.h>

#ifndef MY_HASHTABLE_H
#define MY_HASHTABLE_H

#define SPACE_SIZE 65437

// key should be 32 bits to avoid
// integer overflows during hash calculation
// integer is 16 bit because our space_size is 65437 < 65535
struct Item {
	uint8_t is_set;
	uint8_t is_deleted;
	uint32_t key1;
	uint32_t key2;
	char *info;
	size_t info_length;
	size_t offset;
	uint16_t index1;
	uint16_t index2;
};

struct Table {
	struct Item* space1;
	struct Item* space2;
	size_t space_size;
};

struct ExtendedTable {
	struct Table *table;
	char *filename;
};

struct Tuple {
	uint16_t first;
	uint16_t second;
};

typedef uint32_t (*hash)(uint32_t key);

struct Table* table_create();
struct Tuple get_index(struct Item* space, uint32_t key, hash func, uint8_t key_space);
uint8_t table_insert(struct Table* table, uint32_t key1, uint32_t key2, char *data);
struct Tuple table_find(struct Table *table, uint8_t key_space, uint32_t key);
struct Item* table_get(struct Table *table, uint8_t key_space, uint32_t key);
void item_print(struct Item* item);
uint8_t item_delete(struct Table* table, uint8_t key_space, uint32_t key);
void table_delete(struct Table* table);

struct ExtendedTable* table_load(struct ExtendedTable *ex_table, char *filename);
struct ExtendedTable* table_extend(struct Table* table);
uint8_t table_save(struct ExtendedTable* ex_table);
uint8_t table_fs_insert(struct ExtendedTable* ex_table, uint32_t key1, uint32_t key2, char *data);
struct Item* table_fs_get(struct ExtendedTable *ex_table, uint8_t key_space, uint32_t key);
uint8_t item_fs_delete(struct ExtendedTable* table, uint8_t key_space, uint32_t key);

#endif
