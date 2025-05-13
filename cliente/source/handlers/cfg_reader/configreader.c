#include <stdio.h>
#include <string.h>
#include "configreader.h"
#include <stdlib.h>

#define MAXIMO_CHARS_LINHA 100
#define MAXIMO_CHARS_LINHA_PARAMETRO 50

// Passei um apontador para ConfigCliente para poder alterar/ler o que lá está sem efetuar cópias
void carregarConfigCliente(ConfigCliente *config, const char *ficheiro) {

    FILE *file = fopen(ficheiro, "r");
    
    if (file == NULL) {
        perror("Não foi possível abrir o ficheiro de configuração do cliente!");
        return;
    }

    char line[MAXIMO_CHARS_LINHA];


    while (fgets(line, sizeof(line), file)) {
        char parametro[MAXIMO_CHARS_LINHA_PARAMETRO], valor[MAX_FICHEIRO_NOME_TAM];

        // Efetuar Leitura do Parametro e do valor associado
        //Explicacao REGEX
        // [^:] - Le todos os caracteres que nao seja um :
        // %[^\n] - Le todos os char ate encontrar um espaco \n "Salto de linha"

        sscanf(line, "%[^:]: %[^\n]", parametro, valor);  
        
        // strcmp compara duas strings retorna 0 se forem iguais
        
        if (strcmp(parametro, "IP_SERVIDOR") == 0) {

            strcpy(config->ip, valor);

        } else if (strcmp(parametro, "PORTA_SERVIDOR") == 0) {

            config->porta = atoi(valor);

        } else if (strcmp(parametro, "CLIENTE_ID") == 0) {

            config->cliente_id = atoi(valor);

        } else if (strcmp(parametro, "FICHEIRO_LOG") == 0) {


            strcpy(config->ficheiro_log, valor);
        }
    }

    fclose(file);
}
