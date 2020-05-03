#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "errors.h"

char* errors[MAX_ERRORS]={
    MSG_OK,
    MSG_ERROR_NULL_POINTER,
    MSG_ERROR_FILE_OPENING,
    MSG_ERROR_INVALID_SRV_ARGS,
    MSG_ERROR_INVALID_CLI_ARGS,
    MSG_ERROR_INVALID_FORMAT,
    MSG_ERROR_INVALID_COMMAND,
    MSG_ERROR_VALUE,
    MSG_ERROR_CREATING_SOCKET,
    MSG_ERROR_SOCKET_CONNECTION,
    MSG_ERROR_ADDRESS_INFO,
    MSG_ERROR_SOCKET_CLOSED,
    MSG_ERROR_RECIVING_DATA,
    MSG_ERROR_NEW_CONNECTION,
    MSG_ERROR_BINDING,
    MSG_ERROR_LISTENER,
    MSG_ERROR_NO_VALID_ADDRESSES
};

void print_error(status_t status){
    fprintf(stderr , "%s " , errors[status]);
}
