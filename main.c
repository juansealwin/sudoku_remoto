#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "client.h"
#include "errors.h"

#define POS_MODE 1
#define POS_PORT_SRV 2
#define POS_PORT_CLI 3
#define POS_HOST_CLI 2
#define MAX_ARGS_SRV 3
#define MAX_ARGS_CLI 4

char* dic_formats[]={
    "server",
    "client"
};

status_t validate_arguments(int argc , char* argv[] , config_t* config){
    if (argv == NULL || config == NULL)
        return ERROR_NULL_POINTER;

    if (!strcmp(argv[POS_MODE] , dic_formats[FMT_SRV_MODE])) {
        if (argc == MAX_ARGS_SRV){
            config->format = FMT_SRV_MODE;
            config->port = argv[POS_PORT_SRV];
        } else {
            return ERROR_INVALID_SRV_ARGS;
        }
    } else { 
        if (!strcmp(argv[POS_MODE] , dic_formats[FMT_CLI_MODE])) {
            if (argc == MAX_ARGS_CLI) {
                config->format = FMT_CLI_MODE;
                config->host = argv[POS_HOST_CLI];
                config->port = argv[POS_PORT_CLI];
            } else {
                return ERROR_INVALID_CLI_ARGS;
            }        
        } else {
            return ERROR_INVALID_FORMAT;
        }
    }    
    return OK;
}

int main(int argc , char* argv[]){
    status_t st;
    config_t config;
    client_t client;
    server_t server;

    if ((st = (validate_arguments(argc , argv , &config))) != OK) {
        print_error(st);
        return EXIT_FAILURE;
    }

    if (config.format)
    {
        if ((st = (client_init(&client, config.host, config.port))) != OK) {
            print_error(st);
            client_uninit(&client);
            return EXIT_FAILURE;
        }
        if ((st = (client_transmit_data(&client))) != OK) {
            print_error(st);
            client_uninit(&client);
            return EXIT_FAILURE;
        }
        if ((st = (client_uninit(&client))) != OK) {
            print_error(st);
            return EXIT_SUCCESS;
        }
    } else {
        if ((st = (server_init(&server, config.port))) != OK) {
            print_error(st);
            server_uninit(&server);
            return EXIT_FAILURE;
        }
        if ((st = (server_transmit_data(&server))) != OK) {
            print_error(st);
            server_uninit(&server);
            return EXIT_FAILURE;
        }
        if ((st = (server_uninit(&server))) != OK) {
            print_error(st);
            return EXIT_SUCCESS;
        }
    }
}
