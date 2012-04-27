#include "daemon.h"

int main(void) {
    pid_t pid, sid;

    pid = fork();
    if (pid < 0)
	exit(EXIT_FAILURE);
    if (pid > 0)
	exit(EXIT_SUCCESS);

    pid = getpid();
    printf("Daemons PID: %d\n", pid);

    umask(0);
    sid = setsid();
    if (sid < 0) 
	exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    struct sigaction act_inc, act_dec;
    act_inc.sa_handler = handle_signal;
    sigemptyset(&act_inc.sa_mask);
    act_inc.sa_flags = 0;
   
    act_dec.sa_handler = handle_signal;
    sigemptyset(&act_dec.sa_mask);
    act_dec.sa_flags = 0;
    
    sigaction(SIGUSR1, &act_inc, 0);
    sigaction(SIGUSR2, &act_dec, 0);
    

    for(;;) {
	sleep(1000000);
    }
    exit(EXIT_SUCCESS);
}

void handle_signal(int sig) {
    int speed = 0;
    switch(sig) {
    case SIGUSR1:
	speed = read_conf();
	speed++;
	write_conf(speed);
	break;
    case SIGUSR2:
	speed = read_conf();
	speed--;
	if(speed >= 0) 
	    write_conf(speed);
	break;
    }
}
