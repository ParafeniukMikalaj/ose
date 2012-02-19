#include "stack.h"

stack* get_stack()
{
	stack* instance = (stack*)malloc(sizeof(stack));
	instance->top = -1;
}
void push(stack* st, char value)
{
	st->top++;
	st->values[st->top] = value;
}
char peek(stack* st)
{
	return st->values[st->top];
}
char pop(stack* st)
{
	char c = peek(st);
	st->top--;
	return c;
}
int is_empty(stack* st)
{
	if (st->top < 0)
		return 0;
	return 1;
}
void print_stack(stack* st)
{
	printf("printing stack");
	for(int i = 0; i<=st->top; i++)
		printf(" %c", st->values[i]);
	puts("\n");
}
