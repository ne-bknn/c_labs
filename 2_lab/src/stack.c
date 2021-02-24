#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "lib.h"

int pop(void** p_stack, enum StackType stack_type) {
	switch(stack_type) {
		case List:
			return l_pop((struct ListStack**)p_stack);
		case Array:
			return a_pop((struct ArrayStack**)p_stack);
		default: exit(1);
	}
}

char* top(void *p_stack, enum StackType stack_type) {
	switch(stack_type) {
		case List:
			print_debug("%s\n", "Using List type");
			return l_top((struct ListStack*)p_stack);
		case Array:
			print_debug("%s\n", "Using Array type");
			return NULL;
		default: 
			print_error("Unknown stack type!");
			exit(1);
	}
}

int push(void** p_stack, enum StackType stack_type, char *string) {
	switch(stack_type) {
		case List:
			return l_push((struct ListStack**)p_stack, string);
		case Array:
			return a_push((struct ArrayStack**)p_stack, string);
		default: exit(1);
	}
}

// must be casted to struct in use
void* create_stack(enum StackType stack_type) {
	switch(stack_type) {
		case List:
			return l_create(NULL);
		case Array:
			return a_create(NULL);
		default: exit(1);
	}
}



void stack_free(void* p_stack, enum StackType stack_type) {
	switch(stack_type) {
		case List:
			l_free((struct ListStack*)p_stack);
		case Array:
			a_free((struct ArrayStack*)p_stack);
		default: exit(1);
	}
}

int l_pop(struct ListStack **p_stack) {
	if (NULL == (*p_stack)->previous) {
		return 0;
	}
	struct ListStack *previous_head = (*p_stack)->previous;
	struct ListStack* p_temp = *p_stack;
	*p_stack = previous_head; 
	free(p_temp->string);
	free(p_temp);
	return 1;
}

char* l_top(struct ListStack *p_stack) {
	if (NULL == p_stack->string) {
		print_debug("%s\n", "Stack's string is NULL");
		return 0;
	}
	print_debug("%s\n", "Returning string in head");
	return p_stack->string;
}

int l_push(struct ListStack **p_stack, char *string) {
	if (NULL == (*p_stack)->string) {
		(*p_stack)->string = string;
		return 1;
	}
	struct ListStack* new_node = (struct ListStack*)l_create(string);
	new_node->previous = *p_stack;
	*p_stack = new_node;
	return 1;
}

void* l_create(char* string) {
	struct ListStack* p_stack = malloc(sizeof(struct ListStack));
	p_stack->previous = NULL;
	if (NULL == string) {
		p_stack->string = NULL;
	} else {
		int str_len = strlen(string);
		print_debug("str_len: %d\n", str_len);
		p_stack->string = (char*)malloc(str_len*sizeof(int));
		strncpy(p_stack->string, string, str_len);
	}
	return (void*)p_stack;
}

void l_free(struct ListStack* p_stack) {
	struct ListStack* current_stack = p_stack;
	while (NULL != current_stack) {
		struct ListStack* new_stack = current_stack->previous;
		if (NULL != current_stack->string) {
			free(current_stack->string);
		}
		if (NULL != current_stack) {
			free(current_stack);
		}
		current_stack = new_stack;
	}
}

void* a_create() {
	return NULL;
}

int a_pop(struct ArrayStack** p_stack) {
	return 1;
}

int a_push(struct ArrayStack** p_stack, char *string) {
	return 1;
}

void a_free(struct ArrayStack* p_stack) {
}
