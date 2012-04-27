#include "cloner.h"

int main(void) {
  while(1) {
    int speed = read_conf();
    gen++;
    pid_t pid = getpid();
    sleep(speed);
    printf("PID:%d    Speed:%d    Generation:%d\n", pid, speed, gen);
    pid = fork();
    if (pid < 0)
      exit(EXIT_FAILURE);
    if (pid > 0)
      exit(EXIT_SUCCESS);
  }
}
