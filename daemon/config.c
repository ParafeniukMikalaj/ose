#include "config.h"

int read_conf(void) {
    FILE* fp = fopen(conf_name, "r");
    if (fp == NULL)
	exit(EXIT_FAILURE);
    int speed = -1;
    fscanf(fp, "%d", &speed);
    fclose(fp);
    return speed;
}

void write_conf(int speed) {
    FILE* fp = fopen(conf_name, "w");
    if (fp == NULL)
	exit(EXIT_FAILURE);
    fprintf(fp, "%d", speed);
    fflush(fp);
    fclose(fp);
}
