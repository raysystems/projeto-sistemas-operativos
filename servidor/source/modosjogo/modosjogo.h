#ifndef MODOSJOGO_H
#define MODOSJOGO_H


// sistema de logs
#include "../handlers/log/escrevelogs.h"
// definicao da estrutura da config
#include "../handlers/cfg_reader/configreader.h"
#include "../global.h"
#include "../handlers/print_jogo/printarjogo.h"
#include "../handlers/log/escrevelogs.h"
#include "../handlers/valida_solucao/validarsolucao.h"
#include "../socket/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../libs/JSONLib/cJSON.h"

void modojogoA(char *buffer, int clientID, int sockfd, ConfigServidor configServidor);
void modojogoB(char *buffer, int clientID, int sockfd, ConfigServidor configServidor);
void modojogoC(char *buffer, int clientID, int sockfd, ConfigServidor configServidor); 
void retirar_cliente_pontuacao(int id);
void pontuar_cliente(int id);
void guardarJogo(char modo, char *vencedor);
void despontuar_cliente(int id);
int analizar_cliente_mais_pontuado();
#endif // MODOSJOGO_H