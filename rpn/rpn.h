#include"stack.h"
#include<string.h>
#include<ctype.h>

#define BUFFER_SIZE 255
#define DEBUG 0

char* to_rpn(char* input);
int get_priority(char);
void append(char* s, char c);
