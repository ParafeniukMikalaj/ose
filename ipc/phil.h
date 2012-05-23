#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/sem.h>


#define MIN_TIME 1
#define MAX_TIME 10
#define ARG_LEN 255

char* fifo_name = "phil-fifo";
int fifo_fd;
char buffer[ARG_LEN];
key_t sem_id;
int number;
int sem_nums[2];
int time_to_eat;

void init_phil(int number);
void clear_res();
void main_step();
void grab_sem(int sem_num);
void release_sem();
