#include "pasgen.h"

int main(void) {
    if(mkfifo(fifo_name, 0600) == 0)
	server();
    else if (errno == EEXIST)
	client();
    else
	exit(EXIT_FAILURE);
  
}
void server (void) {
    int fd;
    printf("starting server\n");
    fd = open(fifo_name, O_WRONLY);
    if(fd == -1){
	perror("error opening fifo file");
	exit(EXIT_FAILURE);
    }
	
    while(1) {
	sleep(time_out);
	char* password = generate_password();
	write(fd, password, strlen(password + 1));
    }
}

void client (void) { 
    int fd;
    printf("starting client\n");
    char buffer[pas_len + 1];
    fd = open(fifo_name, O_RDONLY);
    while(1) {
	sleep(time_out);
	if(read(fd, buffer, 100) != 0)
	    printf("received password %s\n", buffer);
    }
}

char* generate_password(void) {
    int i = 0;
    char * pass = malloc((pas_len+1)*sizeof(char));
    pass[pas_len] = 0;
    srandom(time(NULL));
    for (i = 0; i < pas_len; i++) 
	pass[i] =  (char)(26 * ((double)rand() / (RAND_MAX + 1)) + 97);
    return pass;
}
