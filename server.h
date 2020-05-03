#ifndef __SERVER_H__
#define __SERVER_H__

#include "socket.h"
#include "board.h"
#define ROW 1
#define COLUMN 2
#define NUMBER 3
#define MAX_LENGTH_RECV 4
#define MAX_LENGTH_SEND 726
#define MSG_START 4
#define MSG_ERROR_PUT "La celda indicada no es modificable\n\0"
#define MSG_OK_RULES "OK\n\0"
#define MSG_BREACH_OF_RULES "ERROR\n\0"
#define MSG_INVALID_COMMAND "Invalid command\n\0"
#define CHAR_ERROR_COMMAND 16
#define BOARD_LENGTH 722

typedef struct {
    socket_t skt_server;
    socket_t skt_connection;
    board_t game;
}server_t;

//Initialize the server
status_t server_init(server_t * srv, char* port);

//Destroy the server
status_t server_uninit(server_t * srv);

//If the connection is up, it would send data with the client
status_t server_transmit_data(server_t* srv);

//Interpret the message sent from the client
status_t server_read_command(socket_t* skt, uint8_t* buff);

//It prepares the response message to send to the client
status_t server_parse_data(server_t* sv, 
                           uint8_t* buff_rcv,
                           uint8_t* buff_snd,
                           uint8_t* buff_len);

#endif
