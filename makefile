compile: server.c client.c pipe_networking.c
	gcc server.c -o server
	gcc client.c -o client
	gcc pipe_networking.c -o pipe_networking

