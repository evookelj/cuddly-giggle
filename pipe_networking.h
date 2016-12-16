#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

char * WKP_NAME = "luigi";
int client_handshake( int * to_server);
int server_handshake( int * from_client);

