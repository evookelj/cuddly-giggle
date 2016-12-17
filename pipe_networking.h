#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char * WKP_NAME = "luigi";
int MESSAGE_BUFFER_SIZE = 5;
int client_handshake( int * to_server);
int server_handshake( int * from_client);

