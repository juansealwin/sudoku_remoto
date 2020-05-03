#ifndef __TYPES_H__
#define __TYPES_H__

typedef enum{
    FMT_SRV_MODE=0,
    FMT_CLI_MODE=1
}format_t;

typedef struct{
    format_t format;
    char* mode;
    char* host;
    char* port;
}config_t;

typedef enum {
    OK = 0,
    ERROR_NULL_POINTER = 1,
    ERROR_FILE_OPENING = 2,
    ERROR_INVALID_SRV_ARGS = 3,
    ERROR_INVALID_CLI_ARGS = 4,
    ERROR_INVALID_FORMAT = 5,
    ERROR_INVALID_COMMAND = 6,
    ERROR_VALUE = 7,
    ERROR_CREATING_SOCKET = 8,
    ERROR_SOCKET_CONNECTION = 9,
    ERROR_ADDRESS_INFO = 10,
    ERROR_SOCKET_CLOSED = 11,
    ERROR_RECIVING_DATA = 12,
    ERROR_NEW_CONNECTION = 13,
    ERROR_BINDING = 14,
    ERROR_LISTENER = 15,
    ERROR_NO_VALID_ADDRESSES = 16,
    ERROR_FULL_TILE = 17,
    ERROR_RULES = 18
}status_t;

#endif
