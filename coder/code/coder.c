#include"coder.h"

int main (int argc, char *argv[])
{
	int opt, rows, cols;
	char buf[10];
	if(argc < 3) {
		perror("not enough arguments usage: coder rows cols [-r][-d]");
		exit(EXIT_FAILURE);
	}	
	errno = 0;
	rows = strtoint(argv[1]);
	cols = strtoint(argv[2]);
	if(rows == -1 || cols == -1){
		perror("arguments are not in valid format. should be int");
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "de")) != -1) {
		switch(opt) {
		case 'd':
			encrypt(rows, cols);	
			break;
		case 'e':
			encrypt(rows, cols);
			break;
		default:
			
			break;
		}
	}
	return 0;
}
void decrypt(int rows, int columns)
{
	encrypt(columns, rows);
}
void encrypt(int rows, int columns)
{
	int len = rows * columns;
	int first;
	int second;
	int n, j, i;
	char* buf = malloc(sizeof(char) * len);
	char* tmp = malloc(sizeof(char) * len);
	while( (n = fread(buf, sizeof(char), len, stdin)) == len) {
		for(i = 0; i < rows; i++)
			for(j = 0; j < columns; j++){ 
				tmp[j*rows + i] = buf[i*columns + j];
			}
		fwrite(tmp, sizeof(char), len, stdout);			
	}
	tmp[n] = 0;
	for(i = 0; i < n; i++)
		tmp[i] = buf[n-i-1];
	fwrite(tmp, sizeof(char), n, stdout);
}
int strtoint(char* buf)
{
	int res = 0;
	int i = 0;
	while(buf[i] != 0) {
		if(!isdigit(buf[i])) 
			return -1;
		res*=10;
		res+= buf[i] -'0';
		i++;
	}
	return res;
}
