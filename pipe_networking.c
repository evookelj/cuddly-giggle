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

	//4a. client connects to well known pipe
	int opened_wkp = open(WKP_NAME, O_WRONLY);
	printf("[CLIENT] opening WKP\n");
	if (opened_wkp < 0) printf("ERROR: %s\n", strerror(opened_wkp));

	//4b. send the LKP_NAME to the server
	printf("[CLIENT] sending LKP_NAME\n");
	int sent_lkp = write(opened_wkp, LKP_NAME, sizeof(LKP_NAME));
	if (sent_lkp < 0) printf("ERROR: %s\n", strerror(sent_lkp));

	//5. client waits for message from server
	printf("[CLIENT] awaiting server message\n");
	int opened_lkp = open(LKP_NAME, O_RDONLY);
	if (opened_lkp < 0) printf("ERROR: %s\n", strerror(opened_lkp));

	//8. client gets server msg on lkp, removes lkp
	char msg[MESSAGE_BUFFER_SIZE];
	int read_lkp = read(opened_lkp, msg, MESSAGE_BUFFER_SIZE);
	if (read_lkp < 0) printf("ERROR: %s\n", strerror(read_lkp));
	printf("[CLIENT] RECEIVED %s ON LKP\n", msg);
	int removed_lkp = remove(LKP_NAME);
	if (removed_lkp < 0) printf("ERROR: %s\n", strerror(removed_lkp));
	
	return 0;
}

int server_handshake( int * from_client) {
	
	//umasking
	umask(0000);

	//1. server creates a FIFO
	int made_wkp = mkfifo(WKP_NAME, 0644);
	
	//2. open the pipe, block until client connects
	printf("[SERVER] OPENING WKP\n");
	int opened_wkp = open(WKP_NAME, O_RDONLY);
	if (opened_wkp < 0) printf("ERROR: %s\n", strerror(opened_wkp));

	//6. recieve client message, removes WKP
	char lkp_name[MESSAGE_BUFFER_SIZE];
	int read_wkp = read(opened_wkp, lkp_name, MESSAGE_BUFFER_SIZE);
	if (read_wkp < 0) printf("ERROR: %s\n", strerror(read_wkp));
	printf("[SERVER] RECEIVED lkp_name (private) ON WKP\n"); 
	int removed_wkp = remove(WKP_NAME);
	if (removed_wkp < 0) printf("ERROR: %s\n", strerror(removed_wkp));

	//7a. connect to client FIFO
	printf("[SERVER] connecting to LKP\n");
	int opened_lkp = open(lkp_name, O_WRONLY);
	if (opened_lkp < 0) printf("ERROR: %s\n", strerror(opened_lkp));

	//7b. send acknowledgement message on lkp
	printf("[SERVER] sending acknowledgement");
	char msg[MESSAGE_BUFFER_SIZE];
	strcpy(msg, "Hello");
	int sent_msg = write(opened_lkp, msg, sizeof(msg));
	if (sent_msg < 0) printf("ERROR: %s\n", strerror(sent_msg));
		
	return 0;
}
