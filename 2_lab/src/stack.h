#include <stddef.h>

// abstract interface definition

enum StackType {
	List,
	Array
};

int pop(void** p_stack, enum StackType stack_type);
int push(void** p_stack, enum StackType stack_type, char *string);
char* top(void* p_stack, enum StackType stack_type);
void* create_stack(enum StackType stack_type);
void stack_free(void* p_stack, enum StackType stack_type);

// list-based stack
struct ListStack {
	struct ListStack *previous;
	char *string;
};

int l_pop(struct ListStack **p_stack);
int l_push(struct ListStack **p_stack, char *string);
char* l_top(struct ListStack *p_stack);
void* l_create();
void l_free(struct ListStack* p_stack);

// array-based stack
struct ArrayStack {
	char **stack_head;
	size_t max_size;
	size_t filled;
};

int a_pop(struct ArrayStack** p_stack);
int a_push(struct ArrayStack** p_stack, char *string);
void* a_create();
void a_free(struct ArrayStack* p_stack);

