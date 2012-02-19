#include <stdlib.h>
#include <stdio.h>

typedef struct stack {
	int values[100];
	int top;
}stack;

stack* get_stack();
void push(stack*, char value);
char pop(stack*);
char peek(stack*);
int is_empty(stack*);
void print_stack(stack*);
