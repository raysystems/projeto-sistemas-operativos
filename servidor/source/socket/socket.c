#include "socket.h"



int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trinco_jogoiniciado = PTHREAD_MUTEX_INITIALIZER;
int jogoiniciado = 0;
//char **tab_sol;


void broadcast_message(const char *message) {
    for (int i = 0; i < client_count; i++) {
        if (send(client_sockets[i], message, strlen(message), 0) < 0) {
            perror("Erro ao enviar mensagem");
        }
    }
}

void remover_cliente(int sockfd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == sockfd) {
            client_sockets[i] = client_sockets[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Função para tratar lógica do jogo
void tratar_jogo(char *buffer, int clientID, int sockfd, ConfigServidor configServidor) {
    pthread_mutex_lock(&trinco_jogoiniciado);
    if (jogoiniciado == 1) {
        if (configServidor.modo_jogo[0] == 'A') {
            pthread_mutex_unlock(&trinco_jogoiniciado);
            modojogoA(buffer, clientID, sockfd, configServidor);
        } else if (configServidor.modo_jogo[0] == 'B') {
            pthread_mutex_unlock(&trinco_jogoiniciado);
            modojogoB(buffer, clientID, sockfd, configServidor);
        } else if (configServidor.modo_jogo[0] == 'C') {
            pthread_mutex_unlock(&trinco_jogoiniciado);
            modojogoC(buffer, clientID, sockfd, configServidor);
        }
    } else {
        pthread_mutex_unlock(&trinco_jogoiniciado);
    }
}

void gerenciar_desconexao(int sockfd, int clientID, ConfigServidor configServidor) {
    pthread_mutex_lock(&clients_mutex);
    client_count--;

    if (client_count < 2) {
        broadcast_message("terminar");
        escrevelog("Todos os clientes desconectados ou insuficientes para jogar. Enviando kill.", configServidor.ficheiro_log);
        client_count = 0;
    } else {
        char log_msg[256];
        retirar_cliente_pontuacao(clientID);  // Retira o cliente da pontuação
        snprintf(log_msg, sizeof(log_msg), "Cliente ID %d desconectado. Jogadores restantes: %d", clientID, client_count);
        escrevelog(log_msg, configServidor.ficheiro_log);
    }
    pthread_mutex_unlock(&clients_mutex);
}

void gerenciar_jogo_encerrado(int sockfd, int *ja_estou_a_jogar, ConfigServidor configServidor) {
    if (jogoiniciado) {
        broadcast_message("terminar");
        escrevelog("Jogo encerrado por falta de jogadores.", configServidor.ficheiro_log);
        jogoiniciado = 0;
    }

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Cliente ID %d foi informado que o jogo não pode continuar.", sockfd);
    escrevelog(log_msg, configServidor.ficheiro_log);

    *ja_estou_a_jogar = 1; // Marca para não enviar mais mensagens de início
}

// Função para iniciar o jogo
void iniciar_jogo(int *ja_estou_a_jogar, ConfigServidor configServidor) {
    pthread_mutex_lock(&trinco_jogoiniciado);
    if (!jogoiniciado) {
        jogoiniciado = 1;
        broadcast_message("comecou");
        //comecar o timer do tempo
        enviarParaEcra(tabuleiro);
        start_time = time(NULL);
        escrevelog("Jogo iniciado.", configServidor.ficheiro_log);
    }
    pthread_mutex_unlock(&trinco_jogoiniciado);
    *ja_estou_a_jogar = 1;
}

// Envia o ID para o cliente
void enviar_id_cliente(int sockfd, int clientID) {
    char idclt[256];
    snprintf(idclt, sizeof(idclt), "%d", clientID);
    send(sockfd, idclt, strlen(idclt), 0);
}

// Loga a conexão do cliente
void log_conexao_cliente(int clientID, ConfigServidor configServidor) {
    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Novo cliente conectado: ID %d", clientID);
    escrevelog(log_msg, configServidor.ficheiro_log);
}

void *client_handler(void *arg) {
    client_args_t *args = (client_args_t *)arg;
    int sockfd = args->sockfd;
    ConfigServidor configServidor = args->configServidor;
    free(arg);

    pthread_mutex_lock(&clients_mutex);
    int clientID = client_count++;
    client_sockets[clientID] = sockfd;
    pthread_mutex_unlock(&clients_mutex);

    enviar_id_cliente(sockfd, clientID); // Envia o ID para o cliente
    log_conexao_cliente(clientID, configServidor); // Loga a conexão

    int ja_estou_a_jogar = 0;
    char buffer[256];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        // Verifica desconexão ou comando "kill"
        if (bytes_read <= 0 || strcmp(buffer, "kill") == 0) {
            gerenciar_desconexao(sockfd, clientID, configServidor);
            break;
        }

        pthread_mutex_lock(&clients_mutex);
        if (client_count < 2) {
            gerenciar_jogo_encerrado(sockfd, &ja_estou_a_jogar, configServidor);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        } else if (client_count == 2 && !ja_estou_a_jogar) {
            iniciar_jogo(&ja_estou_a_jogar, configServidor);
        } else if (!ja_estou_a_jogar) {
            send(sockfd, "comecou", strlen("comecou"), 0);
            ja_estou_a_jogar = 1;
        }
        pthread_mutex_unlock(&clients_mutex);

        // Lógica de jogo baseada no modo
        // se ja estou a jogar e a mensagem nao contem jogar
        if (ja_estou_a_jogar == 1 && strcmp(buffer,"jogar") != 0) {
            tratar_jogo(buffer, clientID, sockfd, configServidor);
        }
    }

    close(sockfd);
    remover_cliente(sockfd);
    pthread_exit(NULL);
}

void setup_server(int *server_socket, struct sockaddr_in *server_addr,  ConfigServidor *configServidor) {
    if ((*server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(24000);

    if (bind(*server_socket, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        perror("Erro ao associar socket");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(*server_socket, 5000) < 0) {
        perror("Erro ao escutar");
        close(*server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor pronto na porta 24000\n");
}

void accept_clients(int server_socket, ConfigServidor *configServidor) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (new_socket < 0) {
            perror("Erro ao aceitar conexão");
            continue;
        }

        printf("Novo cliente conectado\n");
        client_args_t *client_args = malloc(sizeof(client_args_t));
        client_args->sockfd = new_socket;
        client_args->configServidor = *configServidor;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, client_args) != 0) {
            perror("Erro ao criar thread");
            close(new_socket);
            continue;
        }

        pthread_detach(thread_id);
    }
}