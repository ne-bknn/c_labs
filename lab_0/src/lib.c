#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lib.h"

void print_warning(char msg[]) {
	fprintf(stderr, "[-] %s\n", msg);
}

void print_error(char msg[]) {
	fprintf(stderr, "[!] %s\n", msg);
}

int get_int(int* a) {
	int n = 0;
	while (n == 0) {
		n = scanf("%d", a);
		if (n == 0) {
			print_warning("Error reading integer value");
			scanf("%*[^\n]");
			scanf("%*c");
		}
	}
	// Dont get it
	// What is to be done for n to be
	//
	// Ah got it, Ctrl-D
	return n < 0 ? 0 : 1;
}

bool malformed_input(int a) {
	return (a < 0);
}

// Tuple interface funcs

bool tuples_eq(struct Tuple* a, struct Tuple* b) {
	// a->b is a shorhand for (*a).b, as we accept pointers as arguments
	return (a->a == b->a && a->b == b->b);
}

int tuple_comparator(const void* a, const void* b) {
	int l = ((struct Tuple *)a)->a;
	int r = ((struct Tuple *)b)->a;
	return (l - r);
}

struct Tuple multiply_and_count(int n) {
	struct Tuple answer = {0, 0}; 
	int temp = 0;
	while (n != 0) {
		temp = n % 10;
		if (temp % 2 == 1) {
			++answer.a;
			if (answer.b == 0) {
				answer.b = temp;
			} else {
				answer.b *= temp;
			}
		}
		n /= 10;
	}
	return answer;
}

/* 
* tuple_list funcs
* Stolen from
* https://stackoverflow.com/questions/260915/how-can-i-create-a-dynamically-sized-array-of-structs
*/

tuple_list* create_tuple_list(size_t block_size) {
    tuple_list* pTupleList = malloc(sizeof(tuple_list));
    if (NULL != pTupleList) {
        pTupleList->length= 0;
        pTupleList->size = block_size;
        pTupleList->block_size = block_size;
        pTupleList->tuples= malloc(sizeof(struct Tuple)*block_size);
        if (NULL == pTupleList->tuples) {
            free(pTupleList);
            return NULL;    
        }
    }
    return pTupleList;
}

void delete_tuple_list(tuple_list* pTupleList) {
    free(pTupleList->tuples);
    free(pTupleList);
}

int add_tuple_to_list(tuple_list* pTupleList, struct Tuple tuple) {
    size_t length = pTupleList->length;
    if (length >= pTupleList->size) {
	// Its better to mulpiply by 1.5 or 2 (better amortized performance)
	// Probably will do it at some point
        size_t new_size = pTupleList->size + pTupleList->block_size;
        void* new_tuples = realloc(pTupleList->tuples, sizeof(struct Tuple)*new_size); 
        if (NULL == new_tuples) {
            return 0;
        } else {    
            pTupleList->size = new_size;
            pTupleList->tuples= (struct Tuple*)new_tuples;
        }

    }

    pTupleList->tuples[length] = tuple;
    ++pTupleList->length;

    return 1;
}

struct Tuple* tuple_list_start(tuple_list* pTupleList) {
        return pTupleList->tuples;
}

struct Tuple* tuple_list_end(tuple_list* pTupleList) {
        return &pTupleList->tuples[pTupleList->length];
}

void print_tuple_list(tuple_list* pTupleList) {
	printf("SEQUENCE:\n");
	struct Tuple* iter;
	for (iter = tuple_list_start(pTupleList); iter != tuple_list_end(pTupleList); ++iter) {
		printf("[%d %d] ", iter->a, iter->b);
	}
	printf("\n");
}

// Custom sorting 
// SWAP macro stolen from qsort's source

#define SWAP(a, b, size)                                                      \
  do                                                                              \
    {                                                                              \
      size_t __size = (size);                                                      \
      char *__a = (a), *__b = (b);                                              \
      do                                                                      \
        {                                                                      \
          char __tmp = *__a;                                                      \
          *__a++ = *__b;                                                      \
          *__b++ = __tmp;                                                      \
        } while (--__size > 0);                                                      \
    } while (0)

void bubble_sort_tuple_list(tuple_list* pTupleList) {
	struct Tuple* base_pointer = tuple_list_start(pTupleList);
	int length = pTupleList->length;
	for (int i = 0; i < length - 1; ++i) {
		for (int j = 0; j < length - i - 1; ++j) {
			if ((base_pointer+j)->a > (base_pointer+j+1)->a) {
				SWAP ((void*)(base_pointer+j), (void*)(base_pointer+j+1), sizeof(struct Tuple));
			}
		}
	}
}
