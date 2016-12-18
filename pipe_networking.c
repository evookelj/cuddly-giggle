#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pipe_networking.h"

/*
Basic handshake procedure
1. Server creates a FIFO (Well Known Pipe)
2. Server waits for a connection
3. Client create a “private” FIFO
4a. Client connects to server
4b. Client sends private FIFO name
5. Client waits for a message from the server
6. Server receives client’s message and removes the WKP
7a. Server connects to client FIFO
7b. Server sends an initial acknowledgement message.
8. Client receivers server’s message, removes its private
9. Client sends acknowledgement message.
 */

int client_handshake( int * to_server) {

	//umasking
	umask(0000);

	//3. create a client FIFO, not well known
	char * LKP_NAME = "waluigi";
	int made_lkp = mkfifo(LKP_NAME, 0644);
	if (made_lkp < 0) printf("ERROR on made_lkp: %s\n", strerror(made_lkp));

	//4a. client connects to well known pipe
	*to_server = open(WKP_NAME, O_WRONLY);
	printf("[CLIENT] opening WKP\n");
	if (*to_server < 0) printf("ERROR on *to_server: %s\n", strerror(*to_server));
	
	//4b. send the LKP_NAME to the server
	printf("[CLIENT] sending LKP_NAME\n");
	int sent_lkp = write(*to_server, LKP_NAME, strlen(LKP_NAME)+1);
	if (sent_lkp < 0) printf("ERROR on sent_lkp: %s\n", strerror(sent_lkp));

	//5. client waits for message from server
	printf("[CLIENT] awaiting server message\n");
	int from_server = open(LKP_NAME, O_RDONLY);
	if (from_server < 0) printf("ERROR on from_server: %s\n", strerror(from_server));

	//8. client gets server msg on lkp, removes lkp
	char msg[MESSAGE_BUFFER_SIZE];
	int read_lkp = read(from_server, msg, MESSAGE_BUFFER_SIZE);
	if (read_lkp < 0) printf("ERROR on read_lkp: %s\n", strerror(read_lkp));
	printf("[CLIENT] RECEIVED %s ON LKP\n", msg);
	int removed_lkp = remove(LKP_NAME);
	if (removed_lkp < 0) printf("ERROR on removed_lkp: %s\n", strerror(removed_lkp));

	return from_server;
}
 
int server_handshake( int * from_client) {
	
	//umasking
	umask(0000);

	//1. server creates a FIFO
	int made_wkp = mkfifo(WKP_NAME, 0644);
	
	//2. open the pipe, block until client connects
	printf("[SERVER] OPENING WKP\n");
	* from_client = open(WKP_NAME, O_RDONLY);
	if (*from_client < 0) printf("ERROR on *from_client: %s\n", strerror(* from_client));

	//6. recieve client message, removes WKP
	char lkp_name[MESSAGE_BUFFER_SIZE];
	int read_wkp = read(*from_client, lkp_name, MESSAGE_BUFFER_SIZE);
	if (read_wkp < 0) printf("ERROR on read_wkp: %s\n", strerror(read_wkp));
	printf("[SERVER] RECEIVED lkp_name (private) ON WKP\n"); 
	int removed_wkp = remove(WKP_NAME);
	if (removed_wkp < 0) printf("ERROR on removed_wkp: %s\n", strerror(removed_wkp));

	//7a. connect to client FIFO
	printf("[SERVER] connecting to LKP\n");
	int opened_lkp = open(lkp_name, O_WRONLY);
	printf("opened_lkp: %d\n", opened_lkp);
	if (opened_lkp < 0) printf("ERROR on opened_lkp: %s\n", strerror(opened_lkp));

	//7b. send acknowledgement message on lkp
	printf("[SERVER] sending acknowledgement\n");
	char msg[MESSAGE_BUFFER_SIZE];
	strcpy(msg, "Hello");
	int sent_msg = write(opened_lkp, msg, sizeof(msg));
	if (sent_msg < 0) printf("ERROR on sent_msg: %s\n", strerror(sent_msg));
		
	return opened_lkp;
}
