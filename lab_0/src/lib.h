#include <stdbool.h>
#include <stddef.h>

#ifndef LIB_H
#define LIB_H
// helpers
void print_warning(char msg[]);
void print_error(char msg[]);
int get_int(int* a);
bool malformed_input(int a);

// Tuple interface
struct Tuple {
	int a, b;
};
struct Tuple multiply_and_count(int n);
bool tuples_eq(struct Tuple* a, struct Tuple* b);
int tuple_comparator(const void* a, const void* b);

// tuple_list interface
typedef struct {
    struct Tuple* tuples;
    size_t length;
    size_t size;
    size_t block_size;
} tuple_list;

tuple_list* create_tuple_list(size_t block_size);
void delete_tuple_list(tuple_list* pTupleList);
int add_tuple_to_list(tuple_list* pTupleList, struct Tuple tuple);
struct Tuple* tuple_list_start(tuple_list* pTupleList);
struct Tuple* tuple_list_end(tuple_list* pTupleList);
void print_tuple_list(tuple_list* pTupleList);
void bubble_sort_tuple_list(tuple_list* pTupleList);
#endif
