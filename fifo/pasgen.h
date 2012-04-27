#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#define time_out 1

char* fifo_name = "pasgen";
int pas_len = 8;

void server (void);
void client (void);
char* generate_password(void);
