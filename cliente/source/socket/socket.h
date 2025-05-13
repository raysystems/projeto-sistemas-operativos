#ifndef SOCKET_H
#define SOCKET_H

// INCLUDES PARA O SOCKET
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// sistema de logs
#include "../handlers/log/escrevelogs.h"
// definicao da estrutura da config
#include "../handlers/cfg_reader/configreader.h"

#endif // SOCKET_H


void criarsocket(int *meuid, int *sockfd, struct sockaddr_in *serv_addr, struct hostent *hp, ConfigCliente configCliente);

void fecharsocket(int *sockfd, ConfigCliente configCliente, int meuid);