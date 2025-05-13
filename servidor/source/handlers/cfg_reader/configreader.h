//Evitar includes ciclicos
#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Limitação - Ficheiros não podem ter mais de 50 caracteres
#define MAX_FICHEIRO_NOME_TAM 50


typedef struct {
    char ficheiro_jogo[MAX_FICHEIRO_NOME_TAM];
    int porta;
    char ficheiro_log[MAX_FICHEIRO_NOME_TAM];
    char modo_jogo[1];  
} ConfigServidor;

// Assinatura da função para carregar as configurações do servidor a partir de um ficheiro
void carregarConfigServidor(ConfigServidor *config, const char *filename);

#endif
