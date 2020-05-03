#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <stdlib.h>
#include "types.h"

#define MAX_ERRORS 20
#define MSG_OK "Process done.\n"
#define MSG_ERROR_NULL_POINTER "Error null pointer.\n"
#define MSG_ERROR_FILE_OPENING "Error file opening.\n"
#define MSG_ERROR_INVALID_SRV_ARGS "Use: ./tp server <port>\n"
#define MSG_ERROR_INVALID_CLI_ARGS "Use: ./tp client <host> <port>\n"
#define MSG_ERROR_INVALID_FORMAT \
"Unsupported mode, the first parameter must be server or client\n"
#define MSG_ERROR_INVALID_COMMAND \
"Error, first parameter must be get, put, reset, verify or exit\n"
#define MSG_ERROR_VALUE "Incorrect value\n"
#define MSG_ERROR_CREATING_SOCKET "Error creating socket\n"
#define MSG_ERROR_SOCKET_CONNECTION "Error socket connection\n"
#define MSG_ERROR_ADDRESS_INFO "Error invalid address\n"
#define MSG_ERROR_SOCKET_CLOSED "Error socket has been closed\n"
#define MSG_ERROR_RECIVING_DATA "Error reciving data\n"
#define MSG_ERROR_NEW_CONNECTION "Error establishing new connection\n"
#define MSG_ERROR_BINDING "Error running the server\n"
#define MSG_ERROR_LISTENER "Error running the server\n"
#define MSG_ERROR_NO_VALID_ADDRESSES "Error no valid addresses\n"

//Print the error recived in stderr
void print_error(status_t err);

#endif
