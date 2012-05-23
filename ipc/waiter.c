#include "waiter.h"

int main(void) {
    printf("Waiter started work\n");
    init();
    create_phils();
    main_cycle();
}

void init(){
    int i;
    struct sigaction act;
    act.sa_handler = clear_res;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(sigaction(SIGINT, &act, 0) < 0){
	perror("error in registrating signal\n");
	exit(EXIT_FAILURE);
    }
    key_t sem_key;
    if((sem_key = ftok(sem_path, 132)) == -1){
	perror("canno't create key\n");
	exit(EXIT_FAILURE);
    }
    if((sem_id = semget(sem_key,FORK_COUNT, 0666 |  IPC_CREAT)) <0) {
	perror("Canno't get sem_id\n");
	exit(EXIT_FAILURE);
    }
    printf("semaphore key = %x\n", sem_key);
    for (i =0; i < PHIL_COUNT; i++)
	semctl(sem_id, i, SETVAL);
}

void create_phils() {
    int i;
    for (i = 0; i < PHIL_COUNT; i++) {
	pid_t pid;
	sprintf(buffer, "%s%d", fifo_name, i);
	if (mkfifo(buffer,0600) == -1){
	    perror("error in creation fifo");
	    exit(EXIT_FAILURE);
	}
	if((pid = fork()) < 0){
	    perror("error in fork function");
	    exit(EXIT_FAILURE);
	}
	if(pid==0) {    
	    sprintf(buffer, "%d %d", i, sem_id);
	    execl(PHIL_STRING, PHIL_STRING, buffer, NULL);
	}
	if(pid > 0)
	    phil_pids[i] = pid;
    }
    sleep(1);
    for (i = 0; i < PHIL_COUNT; i++) {
	sprintf(buffer, "%s%d", fifo_name, i);
        int fd = open(buffer, O_WRONLY);
	if(fd == -1){
	    perror("error opening fifo to write");
	    exit(EXIT_FAILURE);
	}
	fifos[i] = fd;
	
    }
}

void main_cycle(){
    int i,j;
    for(i = 0; i < max_steps; i++){
	int fork_count = get_free_forks();
	int phils_count = get_free_phils();
	printf("f: ");
	for(j = 0; j < fork_count; j++)
	    printf("%d", j);
	printf("\n");
	printf("p: ");
	for(j = 0; j < phils_count; j++)
	    printf("%d", j);
	printf("\n");
	while(fork_count >= 2 && phils_count > 0){
	    int first_fork = pick_rand(free_forks, &fork_count);
	    int second_fork = pick_rand(free_forks, &fork_count);
	    int phil = pick_rand(free_phils, &phils_count);
	    give_fork(phil, first_fork, second_fork);
	}
	for (j = 0; j < PHIL_COUNT; j++) 
	    kill(phil_pids[j], SIGUSR1);
	printf("time = %d\n", i);
	sleep(3);
    }
    clear_res(0);
}

void clear_res(int signo) {
    int i;
    printf("\nWaiter is clearing resources\n");
    for (i = 0; i < PHIL_COUNT; i++){
	kill(phil_pids[i], SIGINT);
	close(fifos[i]);
	sprintf(buffer, "%s%d", fifo_name, i);
	unlink(buffer);
    } 
    semctl(sem_id, 0, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}

int get_free_phils(){
    int i, k, len;
    for(i = 0; i < FORK_COUNT; i++)
	busy_phils[i] = -1;    
    for(i = 0, k = 0; i < FORK_COUNT; i++) {
	pid_t current_pid = semctl(sem_id, i, GETPID, 0);
	int phil_index = index_of(phil_pids, current_pid, PHIL_COUNT);
	if (phil_index == -1)
	    continue;
	if(contains(busy_phils,phil_index,FORK_COUNT) == -1)
	    busy_phils[k++] = phil_index;
    }
    len = k;
    for (i = 0, k=0; i < PHIL_COUNT; i++)
	if(contains(busy_phils, i, len) == -1)
	    free_phils[k++] = i;
    return k;
}

int get_free_forks(){
    int i, k;
    k = 0;
    for (i = 0; i<PHIL_COUNT; i++) 
	if(semctl(sem_id, i, GETVAL, 0) >0 )
	    free_forks[k++] = i;	
    return k;
}

void give_fork(int phil, int first_fork, int second_fork){
    sprintf(buffer, "%d %d", first_fork, second_fork);
    write(fifos[phil], buffer, ARG_LEN);
}

int contains(int arr[], int val, int len) {
    int i;
    for(i = 0; i < len; i++)
	if(arr[i] == val)
	    return 0;
    return -1;
}

int index_of(pid_t arr[], pid_t val, int len) {
    int i;
    for(i = 0; i < len; i++)
	if(arr[i] == val)
	    return i;
    return -1;
}

int rand_range(int min, int max) {
    int len = max - min;
    double val = ((double)rand())/RAND_MAX;
    return len*val + min;

}

int pick_rand(int arr[], int *len) {
    int index = rand_range(0, *len-1);
    int tmp = arr[index];
    arr[index] = arr[*len-1];
    (*len)--;
    return tmp;
}
