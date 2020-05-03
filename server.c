#include "server.h"


status_t server_init(server_t* server, char* port){
    if (server == NULL || port == NULL)
        return ERROR_NULL_POINTER;

    status_t st;

    //Initialize the board game
    if ((st = board_init(&server->game)) != OK) {
        return st;
    }

    //Initialize the socket server
    if ((st = socket_new_server(&server->skt_server, port)) != OK) {
        return st;
    }

    //Accept the connection
    if ((st = socket_new_connection(&server->skt_server, 
                                    &server->skt_connection)) != OK) {
        return st;
    }
    return OK;  
}


status_t server_read_command(socket_t* skt, uint8_t* buffer){
    if (skt == NULL || buffer == NULL)
        return ERROR_NULL_POINTER;

    int bytes_recv = 0;
    uint8_t first_char;
    status_t st;

    if ((st = (socket_recive(skt, &first_char, 1))) != OK)
       return st;

    buffer[bytes_recv++] = first_char;

    if (first_char == 'P') {
        if ((st = (socket_recive(skt, &buffer[bytes_recv], NUMBER))) != OK) {
            return st;
        }
    } else {
        buffer[bytes_recv] = '\0';
    }
    return OK;
}

status_t server_transmit_data(server_t* server){   
    bool transmit_data = true;
    uint8_t buffer_recv[MAX_LENGTH_RECV];
    uint8_t buffer_send[MAX_LENGTH_SEND];
    uint8_t buffer_len[sizeof(uint32_t)];
    status_t st;
    
    while (transmit_data) {
        if ((st = (server_read_command(&server->skt_connection, 
                                       buffer_recv))) != OK) {
            if (st == ERROR_SOCKET_CLOSED) {
                transmit_data = false;
                break;
            } else {
                return st;
            }
        }
            
        if ((st = (server_parse_data(server, 
                                     buffer_recv, 
                                     buffer_send, 
                                     buffer_len))) != OK)
            return st;

        if ((st = (socket_send(&server->skt_connection, 
                               buffer_len, 
                               sizeof(uint32_t)))) != OK) {
            if (st == ERROR_SOCKET_CLOSED) {
                transmit_data = false;
                break;
            } else {
                return st;
            }
        }
        
        if ((st = (socket_send(&server->skt_connection, 
                               buffer_send, 
                               strlen((char*)buffer_send)))) != OK) {
            if (st == ERROR_SOCKET_CLOSED) {
                transmit_data = false;
                break;
            } else {
                return st;
            }
        }
    }
    return OK;
}

status_t server_parse_data(server_t* server, 
                           uint8_t* buffer_recv, 
                           uint8_t* buffer_send,
                           uint8_t* buffer_len){ 
    if (buffer_recv == NULL)
        return ERROR_NULL_POINTER;

    status_t st;
    uint32_t len = 0;

    switch (buffer_recv[FIRST_CHAR]) {
        case 'P':

            if ((board_put_value(&(server->game), 
                                 (size_t)(buffer_recv[NUMBER]-48), 
                                 (size_t)(buffer_recv[ROW]-48), 
                                 (size_t)(buffer_recv[COLUMN]-48))) == OK) {
                len = (int)BOARD_LENGTH;
                len = htonl(len);
                memcpy(buffer_len, &len, 4);
                if ((st = (board_check_rules(&server->game, 
                                            &server->game.rules, 
                                            (buffer_recv[NUMBER]-48), 
                                            (buffer_recv[ROW]-48), 
                                            (buffer_recv[COLUMN]-48)))) != OK)
                    return st;
                if ((st = (board_serialize(&server->game, 
                                           buffer_send))) != OK)
                    return st;
            } else {
                len = (int)strlen(MSG_ERROR_PUT);
                len = htonl(len);
                memcpy(buffer_len, &len, 4);
                strncpy((char*)buffer_send, 
                         MSG_ERROR_PUT, 
                         strlen(MSG_ERROR_PUT)+1);          
            }
        break;

        case 'V':
            if (server->game.rules == true) {
                len = (int)strlen(MSG_OK_RULES);
                len = htonl(len);
                memcpy(buffer_len, &len, 4);
                strncpy((char*)buffer_send, 
                         MSG_OK_RULES, 
                         strlen(MSG_OK_RULES)+1); 
            } else {
                len = (int)strlen(MSG_BREACH_OF_RULES);
                len = htonl(len);
                memcpy(buffer_len, &len, 4);
                strncpy((char*)buffer_send, 
                         MSG_BREACH_OF_RULES, 
                         strlen(MSG_BREACH_OF_RULES)+1);
            }
        break;

        case 'R':
            if ((st = (board_reset(&server->game))) != OK)
                return st;
            len = (int)BOARD_LENGTH;
            len = htonl(len);
            memcpy(buffer_len, &len, 4);
            if ((st = (board_serialize(&server->game,
                                       buffer_send))) != OK)
                    return st;        
        break;

        case 'G':
            len = (int)BOARD_LENGTH;
            len = htonl(len);
            memcpy(buffer_len, &len, 4);
            if ((st = (board_serialize(&server->game, buffer_send))) != OK)
                return st;        
        break;

        default:
            len = (int)strlen(MSG_INVALID_COMMAND);
            len = htonl(len);
            memcpy(buffer_len, &len, 4);
            strncpy((char*)buffer_send, 
                     MSG_INVALID_COMMAND, 
                     strlen(MSG_INVALID_COMMAND)+1);   

        break;
    }

    return OK;
}

status_t server_uninit(server_t * server){
    if (server == NULL)
        return ERROR_NULL_POINTER;

    status_t st;

    if ((st = (socket_destroy(&server->skt_server))) != OK) {
        return st;
    }

    if ((st = (socket_destroy(&server->skt_connection))) != OK) {
        return st;
    }
    return OK;  
}
