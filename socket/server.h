#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string.h>
#include <strings.h>

#define MAX_CONN 16
#define MAX_BUF 255


void error(char* message);
int init_server(int port, char* proto);
void serve_requests(int sock_fd, char* proto);
void make_action(int new_sock_fd);
void make_tcp_action(int new_sock_fd);
void make_udp_action(int sock_fd, struct sockaddr_in from, int fromlen);
char* get_message();
void daemonize(void);
