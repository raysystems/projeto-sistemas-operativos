#include "configreader.h"


#define MAXIMO_CHARS_LINHA 100
#define MAXIMO_CHARS_LINHA_PARAMETRO 50

// Passei um apontador para ConfigServidor para poder alterar/ler o que lá está sem efetuar cópids
void carregarConfigServidor(ConfigServidor *config, const char *ficheiro) {

    FILE *file = fopen(ficheiro, "r");
    
    if (file == NULL) {
        perror("Não foi possível abrir o ficheiro de configuração do servidor!");
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
        if (strcmp(parametro, "FICHEIRO_JOGOS") == 0) {

            strcpy(config->ficheiro_jogo, valor);
        } else if (strcmp(parametro, "PORTA") == 0) {
            config->porta = atoi(valor);
        } else if (strcmp(parametro, "LOG_FILE") == 0) {


            strcpy(config->ficheiro_log, valor);
        } else if (strcmp(parametro, "MODO_JOGO") == 0) {


            strcpy(config->modo_jogo, valor);
        }
    }

    printf("Configurações do servidor carregadas com sucesso!\n");
    printf("Ficheiro Jogos: %s\n", config->ficheiro_jogo);
    printf("Porta: %d\n", config->porta);
    printf("Ficheiro Log: %s\n", config->ficheiro_log);
    printf("Modo Jogo: %s\n", config->modo_jogo);

    


    fclose(file);
}
