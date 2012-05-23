#include "phil.h"

int main(int argc, char* argv[]) {
    srandom(time(NULL));
    if(argc != 2) {
	perror("Error in using philosofer\n");
	exit(EXIT_FAILURE);
    }
    sscanf(argv[1], "%d %d", &number, &sem_id);
    sem_nums[0] = -1;
    init_phil(number);    
    for(;;);
}

void init_phil(int number){
    printf("p%d init\n", number);
    struct sigaction clear_act, main_act;
    clear_act.sa_handler = clear_res;
    sigemptyset(&clear_act.sa_mask);
    clear_act.sa_flags = 0;
    if(sigaction(SIGINT, &clear_act, 0) < 0){
	perror("error in registrating signal in philosofer\n");
	exit(EXIT_FAILURE);
    }	

    main_act.sa_handler = main_step;
    sigemptyset(&main_act.sa_mask);
    main_act.sa_flags = 0;
    if(sigaction(SIGUSR1, &main_act, 0) < 0){
	perror("error in registrating signal in philosofer\n");
	exit(EXIT_FAILURE);
    }	

    sprintf(buffer, "%s%d", fifo_name, number);
    if((fifo_fd = open(buffer, O_RDONLY | O_NONBLOCK)) == -1) {
	perror("error opening fifo to read\n");
	exit(EXIT_FAILURE);
    }
}

void clear_res(int signo){
    close(fifo_fd);
}

void main_step(int signo){
    int len;
    if((len = read(fifo_fd, buffer, ARG_LEN)) > 0){
	sscanf(buffer, "%d %d", &sem_nums[0], &sem_nums[1]);
	printf("p%d <- %d %d\n", number, sem_nums[0], sem_nums[1]); 
   	grab_sem(sem_nums[0]);
	grab_sem(sem_nums[1]);
	generate_time();
    } else {
	if(time_to_eat > 0)
	    time_to_eat--;
	if (time_to_eat == 0 && sem_nums[0] != -1){
	    release_sem(sem_nums[0]);
	    release_sem(sem_nums[1]);
	    printf("p%d -> %d %d\n", number, sem_nums[0], sem_nums[1]);
	    sem_nums[0] = sem_nums[1] = -1;
	}
    }
}

void grab_sem(int sem_num) {
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = -1;
    semop(sem_id, &sem_buf, 1);
}

int generate_time(){
    int len = MAX_TIME - MIN_TIME;
    double val = ((double)rand())/RAND_MAX;
    time_to_eat = len*val+MIN_TIME;
    printf("p%d will eat %d sec\n", number, time_to_eat);
}

void release_sem(int sem_num){
    struct sembuf sem_buf;
    sem_buf.sem_num = sem_num;
    sem_buf.sem_op = 1;
    semop(sem_id, &sem_buf, 1);
}
