GCC = gcc

compile: server client

server: server.c pipe_networking.c
	$(GCC) server.c pipe_networking.c -o server

client: client.c pipe_networking.c
	$(GCC) client.c pipe_networking.c -o client

clean:
	rm *.o
	rm server
	rm client

clean-q: server client
	rm server
	rm client
