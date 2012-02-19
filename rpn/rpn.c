#include"rpn.h"

char* to_rpn(char* input)
{
	char* output = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	stack* st = get_stack();
	int priority;
	char top;
	for (int i = 0; i<strlen(input); i++) {
		char c = input[i];
		if(DEBUG)
			printf("current char is %c\n", c);
		if (isalpha(c)) {
			append(output, c);
			if (DEBUG) {
				printf("pushing %c into stack\n", c);
				printf("current string = %s\n",output);
				print_stack(st);
			}
			continue;
		}
		if (!isalpha(c) &&(priority=get_priority(c))==-1)
			return "symbol not supported";
		while (is_empty(st)!=0 && get_priority(top=peek(st)) >= get_priority(c)) {
			if (c == '(') 
				break;
			if (DEBUG) {
				printf("top character is %c\n", top);
			}
			pop(st);
			if (top == '(')
				break;
			append(output, top);
			if (DEBUG)
				printf("%c appended\n", top);
		}	
		if (c != ')') {
			if(DEBUG)
				printf("pushing %c into stack\n", c);
			push(st, c);					
		}
		if (DEBUG) {
			printf("current string = %s\n",output);
			print_stack(st);
		}
	}			
	if (DEBUG)
		printf("no input symbols left emtying stack\n");
	while (is_empty(st) != 0) {
		if (DEBUG)
			printf("pushing %c into stack\n", peek(st));
		append(output, pop(st));
	}
	return output;
}
int get_priority(char c)
{
	switch(c) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	case '(':
	case ')':
		return 0;
	default:
		return -1;
	}	
}
void append(char* s, char c) 
{
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}
