#ifndef PIPE_NETWORK_H
#define PIPE_NETWORK_H
#define MESSAGE_BUFFER_SIZE 50
#define WKP_NAME "luigi"

int client_handshake( int * to_server);
int server_handshake( int * from_client);

#endif

