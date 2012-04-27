CFLAGS += -g
app_name = fifo
source_files = $(wildcard *.c)
obj_files = $(source_files:.c=.o)

$(app_name) : $(obj_files)
	gcc $< -o $@

.PHONY: clean
clean:
	rm -rf *.o *~ $(app_name)