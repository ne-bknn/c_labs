#include <stdio.h>

#include "polytable_interface.h"
#include "polytable.h"
#include "tuilib.h"
#include "utils.h"

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
