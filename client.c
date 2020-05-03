#include "client.h"

char* dic_commands[]={
    "put ",
    "verify",
    "get",
    "reset",
    "exit"
};

status_t client_init(client_t* client, char* ip, char* port){
    if (client == NULL || ip == NULL || port == NULL)
        return ERROR_NULL_POINTER;
    
    status_t st;
    
    if ((st = socket_new_client(&client->skt_client, port, ip)) != OK) {
        return st;
    }
    return OK;
}

bool client_check_values(char* buffer){
    char* delim = "in ";
    char* ptr = strstr(buffer, delim);

    if (!ptr)
        fprintf(stderr , "%s" , MSG_INDEX_ER); 

    if (isdigit(ptr[strlen(delim)]-1) && 
        isdigit(ptr[strlen(delim)+2] -1) && 
               (ptr[strlen(delim)+1] == ',')) {
        if (isdigit(buffer[POS_NUMBER]) && (buffer[POS_NUMBER+1] == ' ')) {
            return true;
        } else {
            fprintf(stderr , "%s" , MSG_VALUE_ER);
        }
    } else {
        fprintf(stderr , "%s" , MSG_INDEX_ER);
    }
    return false;
}

status_t client_parse_data(uint8_t* send_data, bool* client_open){
    if (send_data == NULL || client_open == NULL)
        return ERROR_NULL_POINTER;
    
    char buffer[MAX_INPUT];
    bool wrong_input = true;

    while (wrong_input) {
        wrong_input = false;

        if ((fgets(buffer, MAX_INPUT, stdin)) != NULL) {
            if (strncmp(buffer, 
                        dic_commands[POS_PUT], 
                        strlen(dic_commands[POS_PUT])) == 0) {
                if (client_check_values(buffer)) {
                    send_data[FIRST_CHAR] = 'P';
                    send_data[SECOND_CHAR] = buffer[POS_ROW];
                    send_data[THIRD_CHAR] = buffer[POS_COLUMN];
                    send_data[FORTH_CHAR] = buffer[POS_NUMBER];
                    send_data[FIFTH_CHAR] = '\0';
                } else {
                    //*client_open = false;
                    wrong_input = true;
                }
            } else if (strncmp(buffer, 
                               dic_commands[POS_VERIFY], 
                               strlen(dic_commands[POS_VERIFY])) == 0) {
                send_data[FIRST_CHAR] = 'V';
                send_data[SECOND_CHAR] = '\0';
            } else if (strncmp(buffer, 
                               dic_commands[POS_GET], 
                               strlen(dic_commands[POS_GET])) == 0) {
                send_data[FIRST_CHAR] = 'G';
                send_data[SECOND_CHAR] = '\0';
            } else if (strncmp(buffer, 
                               dic_commands[POS_RESET], 
                               strlen(dic_commands[POS_RESET])) == 0) {
                send_data[FIRST_CHAR] = 'R';
                send_data[SECOND_CHAR] = '\0';
            } else if (strncmp(buffer, 
                               dic_commands[POS_EXIT], 
                               strlen(dic_commands[POS_EXIT])) == 0) {
                *client_open = false;
            } else {
                //If the input was unsupported, starts again
                wrong_input = true;
                fprintf(stderr , "%s" , MSG_UNSUPPORTED_INPUT);
            }
        } else {
            *client_open = false;
        }
    }
    return OK;
}

status_t client_transmit_data(client_t* client){    
    bool client_open = true;
    uint8_t buffer_recv[MAX_DATA_RECV];
    uint8_t send_data[LEN_SIZE];
    int length;
    status_t st;

    while (client_open) {
        if ((st = client_parse_data(send_data, &client_open)) != OK) {
            return st;
        }
        if (!client_open)
            break; 
        length = 0;
        if ((st = socket_send(&client->skt_client,
                              send_data, 
                              strlen((char*)send_data))) != OK) {
            if (st == ERROR_SOCKET_CLOSED) {
                client_open = false;
                break;
            } else {
                return st;
            }
        }
        if ((st = client_read_data(&client->skt_client, 
                                   buffer_recv, 
                                   &length)) != OK) {
            if (st == ERROR_SOCKET_CLOSED) {
                client_open = false;
                break;
            } else {
                return st;
            }
        }
        if ((st = client_print_msg(buffer_recv, length)) != OK) {
            return st;
        }
    }
    return OK;
}

status_t client_read_data(socket_t* skt, 
                          uint8_t buffer_recv[MAX_DATA_RECV],
                          int* length){
    if (skt == NULL || buffer_recv == NULL)
        return ERROR_NULL_POINTER;

    uint8_t buffer_len[sizeof(uint32_t)];
    status_t st;

    if ((st = socket_recive(skt, buffer_len, sizeof(uint32_t))) != OK)
        return st; 

    if ((st = (client_msg_length(buffer_len, length))) != OK)
        return st;

    if ((st = socket_recive(skt, buffer_recv, *length)) != OK)
        return st;    

    return OK;
}

status_t client_msg_length(uint8_t* buffer, int* length){
    if (buffer == NULL || length == NULL)
        return ERROR_NULL_POINTER;

    *length = buffer[FORTH_CHAR] + buffer[THIRD_CHAR]*MAX_RANGE 
    + buffer[SECOND_CHAR]*MAX_RANGE*MAX_RANGE 
    + buffer[FIRST_CHAR]*MAX_RANGE*MAX_RANGE*MAX_RANGE;

    return OK;
}

status_t client_print_msg(uint8_t* buffer, int length){
    if (buffer == NULL)
        return ERROR_NULL_POINTER;

    for (size_t i = 0 ; i < length ; i++) {
        printf("%c", buffer[i]);
    }
    return OK;
}

status_t client_uninit(client_t * client){
    if (client == NULL)
        return ERROR_NULL_POINTER;
    
    status_t st;

    if ((st = (socket_destroy(&client->skt_client))) != OK) {
        return st;
    }
    return OK;  
}
