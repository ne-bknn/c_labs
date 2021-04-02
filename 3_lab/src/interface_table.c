#include <string.h>
#include <stdint.h>

#include "utils.h"
#include "tuilib.h"
#include "table.h"
#include "interface_table.h"

void tuilib_item_delete(void **callback_data, void *main_structure) {
	print_debug("%s", "In tuilib_item_delete");
	struct Table *table = (struct Table*)main_structure;
	uint8_t keyspace = (uint8_t)(*(int*)callback_data[0]);
	if (keyspace != 1 && keyspace != 2) {
		msg_warn("Got wrong keyspace!");
		return;
	}
	uint32_t key = (uint32_t)(*(int*)callback_data[1]);
	int status = item_delete(table, keyspace, key);
	printf("Execution status: %d\n", status);
}

void tuilib_item_get(void **callback_data, void *main_structure) {
        struct Table *table = (struct Table*)main_structure;
        print_debug("%s", "Item get");                         
        uint8_t keyspace = (uint8_t)(*(int*)callback_data[0]);
	print_debug("%s", "1234");
	if (keyspace != 1 && keyspace != 2) {
		msg_warn("Got wrong keyspace!");
		return;
	}
        uint32_t key = (uint32_t)(*(int*)callback_data[1]);
	print_debug("%s", "111111");
        struct Item* item = table_get(table, keyspace, key);
	if (NULL == item) {
		msg_warn("No such key exists!");
		return;
	}
	item_print(item);
}
/*
void tuilib_table_save(void **callback_data, void *main_structure) {
	struct ExtendedTable *ex_table = (struct ExtendedTable*)main_structure;
}
*/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void tuilib_table_print(void **callback_data, void *main_structure) {
#pragma clang diagnostic pop
	struct Table* table = (struct Table*)main_structure;
	for (size_t i = 0; i < table->space_size; ++i) {
		if (table->space1[i].is_set != 0) {
			print_debug("is_set: %d", table->space1[i].is_set);
			item_print(&table->space1[i]);
		}
	}
}


void tuilib_table_insert(void **callback_data, void *main_structure) {
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
	int status = table_insert(table, key1, key2, new_data);
	printf("Execution status: %d\n", status);
}
