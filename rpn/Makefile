CFLAGS += -g -std=c99
appname = rpn 
obj-files = $(addsuffix .o,$(basename $(wildcard *.c)))

$(appname): $(obj-files)
	gcc $(LDFLAGS) $(obj-files) -o $@


.PHONY: clean
clean: clean-obj
	rm -f $(appname)
.PHONY: clean-obj
clean-obj:
	rm -f *.o *~
