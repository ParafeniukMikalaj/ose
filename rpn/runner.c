#include"rpn.h"

int main() {
	puts("input some expression");
	char* input = (char*)malloc(sizeof(char)*BUFFER_SIZE);
	gets(input);
	char* output = to_rpn(input);
	puts("result");
	puts(output);
};
