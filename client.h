#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "socket.h"
#include <ctype.h>

#define MAX_INPUT 15
#define POS_PUT 0
#define POS_VERIFY 1
#define POS_GET 2
#define POS_RESET 3
#define POS_EXIT 4
#define POS_NUMBER 4
#define POS_ROW 9
#define POS_COLUMN 11
#define CHAR_1 48
#define CHAR_9 58
#define MAX_DATA_RECV 726
#define MAX_RANGE 256
#define MSG_INDEX_ER "Error en los índices. Rango soportado: [1,9]\n\0"
#define MSG_VALUE_ER "Error en el valor ingresado. Rango soportado: [1,9]\n\0"
#define MSG_UNSUPPORTED_INPUT "Error, unsupported input​\n\0"

typedef struct {
    socket_t skt_client;
}client_t;

//Initialize the client
status_t client_init(client_t* client, char* ip, char* port);

//Destroy the client
status_t client_uninit(client_t* client);

//If the connection is active, the client would be sending data to the server
status_t client_transmit_data(client_t* client);

//Load the buffer with the data sent from the server 
status_t client_read_data(socket_t* skt, 
                          uint8_t buff[MAX_DATA_RECV], 
                          int* length);

//Serialize the data from the stdin to be sent to the server
status_t client_parse_data(uint8_t* send_data, bool* cli_open);

//Reads the first 4 bytes to know the length of data that would be reciving
status_t client_msg_length(uint8_t* buff, int* len);

//Prints the message recived from the server
status_t client_print_msg(uint8_t* buffer, int length);

//Checks if the values in the stdin were numbers
bool client_check_values(char* buffer);

#endif
