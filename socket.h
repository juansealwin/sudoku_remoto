#ifndef __SOCKET_H__
#define __SOCKET_H__

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include "types.h"
#define FIRST_CHAR 0
#define SECOND_CHAR 1
#define THIRD_CHAR 2
#define FORTH_CHAR 3
#define FIFTH_CHAR 4
#define LEN_SIZE 4

typedef struct {
    int file_desc;
}socket_t;

//Destroy a socket
status_t socket_destroy(socket_t* skt);

//Initialize the socket of the client with a specific port and ip
status_t socket_new_client(socket_t* skt, char* port, char* ip);

//Initialize the socket of the server with a specific ip
status_t socket_new_server(socket_t* skt_srv, char* port);

//Tries to establish a new connection with two sockets
status_t socket_new_connection(socket_t* skt_srv, socket_t* new_skt);

//Recive data from a specific socket
status_t socket_recive(socket_t* skt, uint8_t* buff, int len);

//Send data to a specific socket
status_t socket_send(socket_t* skt, uint8_t* buff, size_t len);

#endif
