#include "client.h"

char buffer[MAX_BUF];
int n;
struct sockaddr_in serv_addr;

int main(int argc, char* argv[]) {
    if(argc != 4)
	error("usage: host port [tcp, udp]");
    char* host = argv[1];
    int port = atoi(argv[2]);
    char* proto = argv[3];
    int sock_fd = init_client(host, port, proto);
    test_request(sock_fd, proto);
}

int init_client(char* host, int port, char* proto) {
    int sock_fd;   

    if(strcmp(proto, "tcp") != 0 && strcmp(proto, "udp") != 0) {
	printf("unknown protocol %s\n", proto);
	error("select tcp or udp");
    }

    if(strcmp(proto, "tcp") == 0)
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(strcmp(proto, "udp") == 0)	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0); 

    if (sock_fd < 0) 
	error("ERROR opening socket");

    struct hostent *server = gethostbyname(host);
    if (server == NULL) 
	error("ERROR no such host\n");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if(connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	error("ERROR connecting");

    return sock_fd;
 }

int test_request(int sock_fd, char* proto) {
     if (strcmp(proto, "tcp") == 0){
	n = write(sock_fd,buffer,1);
	if (n < 0) 
	    error("ERROR writing to socket");

	bzero(buffer,MAX_BUF);

	n = read(sock_fd,buffer,MAX_BUF-1);
	if (n < 0) 
	    error("ERROR reading from socket");
    }
    if (strcmp(proto, "udp") == 0){
	int length = sizeof(struct sockaddr_in);
	n=sendto(sock_fd,buffer, strlen(buffer),0,(const struct sockaddr *)&serv_addr,length);

	if (n < 0) 
	    error("ERROR sendto");

	n = recvfrom(sock_fd,buffer,MAX_BUF,0,NULL, NULL);
	if (n < 0) 
	    error("ERROR recvfrom");
    }
    printf("%s\n", buffer);
    close(sock_fd);
    return 0;
}

void error (char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}
