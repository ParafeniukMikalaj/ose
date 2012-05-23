CFLAGS += -g
app_name = ipc
phil_name = phil
source_files = $(wildcard *.c)
obj_files = $(source_files:.o=.c)

$(app_name): $(phil_name) waiter.o
	gcc waiter.o -o $(app_name)

$(phil_name): phil.o
	gcc $< -o $(phil_name)

.PHONY: clean
clean:
	rm -rf *.o *~ $(app_name) $(phil_name) phil-fifo*