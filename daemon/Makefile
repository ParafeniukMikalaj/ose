CFLAGS += -g
daemon_name = daemon
runner_name = cloner
config_name = config
source_files = $(wildcard *.c)
obj_files = $(source_files:.c=.o)

build_all: $(runner_name) $(daemon_name)

$(runner_name): cloner.o config.o
	gcc $^ -o $@

$(daemon_name):daemon.o config.o
	gcc $^ -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ $(daemon_name) $(runner_name) $(config_name)
