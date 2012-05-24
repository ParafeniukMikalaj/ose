#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <strings.h>
#include <string.h>

#define MAX_BUF 255

int init_client(char* hostname, int port, char* proto);
int test_request(int sock_fd, char* proto);
void error(char* message);
