#include "escrevelogs.h"
void escrevelog(char *mensagem, char *ficheiro) {

    FILE *arq;
    arq = fopen(ficheiro, "a");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo de log\n");
        exit(1);
    }


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char buffer[100];


    strftime(buffer, sizeof(buffer), "[ %H:%M:%S - %d/%m/%Y ]", &tm);

    // Mensagem ja no formato escolhido [ HH:MM:SS - 12/09/2024 ] - MOTIVO

    fprintf(arq, "%s - %s\n", buffer, mensagem);
    
    fclose(arq);

}