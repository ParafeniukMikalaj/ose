CFLAGS += -g
client_name = client
server_name = server
client_obj = $(addsuffix .o, $(client_name))
server_obj = $(addsuffix .o, $(server_name))

$(client_name) : $(server_name) $(client_obj)
	gcc $(LDFLAGS) $(client_obj) -o $(client_name)

$(server_name) : $(server_obj)
	gcc $(LDFLAGS) $(server_obj) -o $(server_name)

.PHONY: clean
clean:
	rm -rf *.o *~ $(server_name) $(client_name)