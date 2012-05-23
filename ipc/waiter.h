#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PHIL_STRING "./phil"
#define PHIL_COUNT 5
#define FORK_COUNT 5
#define max_steps 100
#define ARG_LEN 255

char buffer[ARG_LEN];
int fifos[PHIL_COUNT];
pid_t phil_pids[PHIL_COUNT];
int free_phils[PHIL_COUNT];
int busy_phils[PHIL_COUNT];
int free_forks[FORK_COUNT];
int sem_id;
int fifo_fd;

char* fifo_name = "phil-fifo";
char* sem_path = "ipc";
void init();
void create_phils();
void clear_res();
void main_cycle();
int get_free_phils();
int get_free_forks();
void give_fork(int phil, int first_fork, int second_fork);
int contains(int arr[], int val, int len);
int index_of(pid_t arr[], pid_t val, int len);
int rand_range(int min, int max);
int pick_rand(int arr[], int *len);
