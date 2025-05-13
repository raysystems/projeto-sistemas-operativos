#include "socket.h"




void err_dump(const char *msg) {
    perror(msg);
    exit(1);
}

void criarsocket(int *meuid, int *sockfd, struct sockaddr_in *serv_addr, struct hostent *hp, ConfigCliente configCliente) {
    /* Cria socket stream */

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err_dump("client: can't open stream socket");

        escrevelog("Houve um erro a tentar estabelecer comunicação com o servidor. Can't open stream socket", configCliente.ficheiro_log);
    } 

    /* Limpar para prevenir */
    bzero((char *)serv_addr, sizeof(struct sockaddr_in));
    serv_addr->sin_family = AF_INET;


    char *server_ip = configCliente.ip;

    /* Converte o IP para a struct in_addr */
    if (inet_pton(AF_INET, server_ip, &serv_addr->sin_addr) <= 0) {
        perror("inet_pton error");
        escrevelog("Houve um erro a tentar estabelecer comunicação com o servidor. Não foi possível obter o IP", configCliente.ficheiro_log);

        exit(1);
    }

    /* Configura a porta do servidor */
    serv_addr->sin_port = htons(configCliente.porta);

    /* Conecta ao servidor */
    if (connect(*sockfd, (struct sockaddr *)serv_addr, sizeof(struct sockaddr_in)) < 0) {
        err_dump("client: can't connect to server");
        escrevelog("Houve um erro a tentar estabelecer comunicação com o servidor. Canal de Comunicação não foi Aberto!", configCliente.ficheiro_log);
    } else {
        // Correu tudo bem vamos so escrever nos logs
        escrevelog("Correu tudo bem na criação do socket com o servidor. Canal de Comunicação Aberto!", configCliente.ficheiro_log);
    }
    
    // Receber o ID do servidor
    char buffer[1024];
    int bytes_recebidos = recv(*sockfd, buffer, sizeof(buffer), 0);
    if (bytes_recebidos > 0) {
        buffer[bytes_recebidos] = '\0';
   
    }
    *meuid = atoi(buffer);
    printf("MEU ID: %d\n", *meuid);
    
    
}

void fecharsocket(int *sockfd, ConfigCliente configCliente, int meuid) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "O socket foi terminado! Cliente ID: %d", meuid);
    escrevelog(log_msg, configCliente.ficheiro_log);
    close(*sockfd);
}