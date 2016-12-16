#include "pipe_networking.h"

int client_handshake( int * to_server) {

	//umasking
	umask(0000);

	//create a client FIFO, not well known
	char * LKP_NAME = "waluigi";
	int made_wkp = mkfifo(LKP_NAME, 0644);

	//client connects to well known pipe
	int opened_pipe = open(WKP_NAME, O_WRONLY);
	if (opened_pipe < 0) printf("ERROR: %s\n", strerror(opened_pipe));

	//send the LKP_NAME to the server
	int sent_LKP = write(opened_pipe, LKP_NAME, sizeof(LKP_NAME));
	if (sent_LKP < 0) printf("ERROR: %s\n", strerror(sent_LKP));

	//	
	return 0;
}

int server_handshake( int * from_client) {
	
	//umasking
	umask(0000);

	//create a FIFO
	int made_wkp = mkfifo(WKP_NAME, 0644);
	
	//open the pipe, block until client connects
	int opened_pipe = open(WKP_NAME, O_RDONLY);
	if (opened_pipe < 0) printf("ERROR: %s\n", strerror(opened_pipe));
		
	return 0;
}
