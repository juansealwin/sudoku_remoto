#include "socket.h"

status_t socket_destroy(socket_t* skt){  
    if (skt == NULL)
        return ERROR_NULL_POINTER;

    shutdown(skt->file_desc, SHUT_RDWR);
    close(skt->file_desc);
    
    return OK;
}

status_t socket_new_client(socket_t* skt_cli, char* port, char* ip){
    if (skt_cli == NULL || port == NULL || ip == NULL)
         return ERROR_NULL_POINTER;

    skt_cli->file_desc = 0;

    struct addrinfo hints;
    struct addrinfo* res;
    struct addrinfo* ptr;
    bool are_we_connected = false;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP  
    hints.ai_flags = 0;              // None (or AI_PASSIVE for server) 

    if (getaddrinfo(ip, port, &hints, &res) != 0) { 
        return ERROR_ADDRESS_INFO;
    }

    for (ptr = res; 
    ptr != NULL && are_we_connected == false; 
    ptr = ptr->ai_next){
        skt_cli->file_desc = socket(ptr->ai_family, 
                                ptr->ai_socktype, 
                                ptr->ai_protocol);
        if (skt_cli->file_desc == -1) {
            return ERROR_CREATING_SOCKET;
        } else if (connect(skt_cli->file_desc, 
                            ptr->ai_addr, 
                            ptr->ai_addrlen) == -1) {
            close(skt_cli->file_desc);
            return ERROR_SOCKET_CONNECTION;   
        } else {
            are_we_connected = true; 
        }
    }
    
    freeaddrinfo(res);

    if (are_we_connected == false) {
        return ERROR_NO_VALID_ADDRESSES;
    }
    return OK;
}

status_t socket_new_server(socket_t* skt_svr, char* port){
    if (skt_svr == NULL || port == NULL)
        return ERROR_NULL_POINTER;

    struct addrinfo* ptr;
    struct addrinfo* res;
    struct addrinfo hints;
    bool are_we_connected = false;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP  
    hints.ai_flags = 0;              // None (or AI_PASSIVE for server) 

    if (getaddrinfo(0, port, &hints, &res) != 0) { 
        return ERROR_ADDRESS_INFO;
    }

    for (ptr = res; 
    ptr != NULL && are_we_connected == false; 
    ptr = ptr->ai_next){
        skt_svr->file_desc = socket(ptr->ai_family, 
                                ptr->ai_socktype, 
                                ptr->ai_protocol);
        if (skt_svr->file_desc == -1) {
            return ERROR_CREATING_SOCKET;
        } else if (bind(skt_svr->file_desc, 
                        res->ai_addr, 
                        res->ai_addrlen) == -1) {
            freeaddrinfo(res);
            return ERROR_BINDING;  
        } else {
            are_we_connected = true; 
        }
    }
    
    freeaddrinfo(res);

    if (listen(skt_svr->file_desc, 5) == -1) {
        return ERROR_LISTENER;
    }
    return OK;
}

status_t socket_new_connection(socket_t* skt_srv, socket_t* new_skt){
    if (skt_srv == NULL || new_skt == NULL)
        return ERROR_NULL_POINTER;

    if ((new_skt->file_desc = (accept(skt_srv->file_desc, NULL, NULL))) == -1) {
        return ERROR_NEW_CONNECTION;
    }
    return OK;
}

status_t socket_send(socket_t* skt, uint8_t* buffer, size_t length){
    if (skt == NULL || buffer == NULL)
        return ERROR_NULL_POINTER;

    int value_send = 0;
    int bytes_sent = 0;

    while (bytes_sent < length) {
        value_send = send(skt->file_desc, 
                     (char*)&buffer[bytes_sent], 
                     length - bytes_sent, 
                     MSG_NOSIGNAL);
        if (value_send > 0) {
           bytes_sent += value_send;
        } else if (value_send == 0) {
            return ERROR_SOCKET_CLOSED;
        } else {
            return ERROR_RECIVING_DATA;
        } 
    }
    return OK;
}

status_t socket_recive(socket_t* skt, uint8_t* buffer, int length){
    if (skt == NULL || buffer == NULL)
        return ERROR_NULL_POINTER;
    
    size_t value_recv = 0;
    size_t bytes_recv = 0;
   
    while (bytes_recv < length) {
        value_recv = recv(skt->file_desc, 
                         (char*)&buffer[bytes_recv], 
                         length - bytes_recv, 
                         MSG_NOSIGNAL);
        if (value_recv > 0) {
            bytes_recv += value_recv;
        } else if (value_recv == 0) {
            return ERROR_SOCKET_CLOSED;
        } else {
            return ERROR_RECIVING_DATA;
        } 
    } 
    return OK;
}
