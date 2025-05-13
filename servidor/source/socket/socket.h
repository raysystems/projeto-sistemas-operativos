#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>
#include "../handlers/cfg_reader/configreader.h"
#include "../handlers/log/escrevelogs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "../global.h"
#include "../handlers/print_jogo/printarjogo.h"
#include "../modosjogo/modosjogo.h"

// Declarações das funções
void broadcast_message(const char *message);
void remover_cliente(int sockfd);
void *client_handler(void *arg);
void setup_server(int *server_socket, struct sockaddr_in *server_addr,  ConfigServidor *configServidor);
void accept_clients(int server_socket,  ConfigServidor *configServidor);
typedef struct {
    int sockfd;
    ConfigServidor configServidor;
} client_args_t;
#endif // SOCKET_H

