//Evitar includes ciclicos
#ifndef CONFIGREADER_H
#define CONFIGREADER_H

//Limitação - Ficheiros não podem ter mais de 50 caracteres
#define MAX_FICHEIRO_NOME_TAM 50

// IP é desta forma XXX.XXX.XXX.XXX no máximo 15 caracteres
#define MAX_IP_TAM 15
typedef struct {
    char ip[MAX_IP_TAM];
    int porta;
    int cliente_id;
    char ficheiro_log[MAX_FICHEIRO_NOME_TAM];
} ConfigCliente;

// Assinatura da função para carregar as configurações do cliente a partir de um ficheiro
void carregarConfigCliente(ConfigCliente *config, const char *filename);

#endif
