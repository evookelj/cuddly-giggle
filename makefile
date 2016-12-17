GCC = gcc -c

compile: server.c client.c pipe_networking.c
	$(GCC) pipe_networking.c
	gcc client.c -o client
	gcc server.c -o server

clean:
	rm *.o

