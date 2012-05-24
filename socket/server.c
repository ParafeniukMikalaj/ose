#include "server.h"

char buffer[MAX_BUF];
int n;

int main(int argc, char* argv[]) {

    if(argc != 3){
	printf("usage: %s port [tcp, udp]\n", argv[0]);
	error("error in start parameters");
    }

    signal(SIGCHLD, SIG_IGN);

    int port = atoi(argv[1]);
    char* proto = argv[2];
    int sock_fd = init_server(port, proto);

    daemonize();

    serve_requests(sock_fd, proto);
}

void daemonize(void) {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0)
	error("ERROR fork in daemonize()");
    if (pid > 0)
	exit(EXIT_SUCCESS);

    pid = getpid();
    printf("Server started. PID: %d\n", pid);

    umask(027);
    sid = setsid();
    if (sid < 0) 
	exit(EXIT_FAILURE);

    chdir("/");
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO); 
}

int init_server(int port, char* proto){
    int sock_fd;
    struct sockaddr_in serv_addr;
    int serv_len = sizeof(serv_addr);
    int reuse = 1;

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

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sock_fd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
	error("ERROR setting reusable socket");
	return sock_fd;
}

void serve_requests(int sock_fd, char* proto) {
    struct sockaddr_in cli_addr;
    pid_t pid;
    int  new_sock_fd, clilen;

    if(strcmp(proto, "tcp") == 0){
	listen(sock_fd, MAX_CONN);
	clilen = sizeof(cli_addr);
	while(1) {
	    new_sock_fd = accept(sock_fd,(struct sockaddr *) &cli_addr, &clilen);
	    if (new_sock_fd < 0) 
		error("ERROR on accept");

	    pid = fork();
	    if (pid < 0)
		error("ERROR on fork");
	    if (pid == 0)  {
		close(sock_fd);
		make_tcp_action(new_sock_fd);
		exit(0);
	    }
	    else close(new_sock_fd);

	}
    }

    if(strcmp(proto, "udp") == 0){
	int fromlen = sizeof(struct sockaddr_in);
	while (1) {
	    n = recvfrom(sock_fd, buffer, MAX_BUF, 0, (struct sockaddr *)&cli_addr, &clilen);
	    if (n < 0) 
		error("recvfrom");

	    pid = fork();
	    if (pid < 0)
		error("ERROR on fork");
	    if (pid == 0)  {
		make_udp_action(sock_fd, cli_addr, clilen);
		close(sock_fd);
		exit(0);
	    }	   

	}
    }

    printf("unknown protocol %s\n", proto);
    error("ERROR select tcp or udp\n");
}

void make_tcp_action(int new_sock_fd) {
    bzero(buffer, MAX_BUF);
    n = read(new_sock_fd, buffer, MAX_BUF);
    if (n < 0) 
	error("ERROR reading from socket");
    printf("TCP request received\n");
    char* message = get_message();
    n = write(new_sock_fd, message, strlen(message));
    if (n < 0) 
	error("ERROR writing to socket");
}

void make_udp_action(int sock_fd, struct sockaddr_in from, int fromlen){
    printf("UDP request received\n");
    char* message = get_message();
    n = sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&from, fromlen);
    if (n  < 0) 
	error("ERROR sendto");
}

char* get_message(){
    time_t unix_era = 2208988800;
    time_t current_time = time(NULL);
    time_t time = unix_era + current_time;
    sprintf(buffer, "Current time: %ld", time);
    return buffer;
}

void error (char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}
