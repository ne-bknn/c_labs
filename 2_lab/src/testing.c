#include "stack.h"
#include <stdio.h>

int main() {
	enum StackType type = List;
	void* stack = create_stack(type);
	push(&stack, type, "hhello");
	printf("Top: %s\n", top(stack, type));
	push(&stack, type, "world");
	printf("Top: %s\n", top(stack, type));
	pop(&stack, type);
	printf("Top after pop: %s\n", top(stack, type));
	l_free(stack);
	return 0;
}
