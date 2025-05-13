#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

// sistema de logs
#include "../handlers/log/escrevelogs.h"
// definicao da estrutura da config
#include "../handlers/cfg_reader/configreader.h"

#endif // JOGO_H

int iniciarJogo(int sockfd, ConfigCliente configCliente , int *jogo_iniciado, pthread_mutex_t *trinco_jogo, int meuid);

void enviarEscutarCasasParaEnviarServidor(int sockfd, int *estadojogo, pthread_mutex_t *trinco_jogo, ConfigCliente configCliente, int meuid);