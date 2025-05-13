#include <stdio.h>
#include "handlers/cfg_reader/configreader.h"
#include "handlers/print_jogo/printjogo.h"
#include "handlers/log/escrevelogs.h"
#include "socket/socket.h"
#include "jogo/jogo.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include <signal.h>
#include <pthread.h>

int sockfd;
int meuid = -1;
int jogo_iniciado = 0;
struct sockaddr_in serv_addr;
struct hostent *hp;
ConfigCliente configCliente;
pthread_mutex_t trinco_jogo = PTHREAD_MUTEX_INITIALIZER;
int terminado = 0;

void mandarparaecra(char *buffer)
{
    char *token = strtok(buffer, ":");
    char *jogo = token;
    token = strtok(NULL, ":");
    char *valor = token;
    printf("\033[H\033[J");
    enviarParaEcra(valor);
}

void *RecebeInformacao()
{
    char buffer[4096]; // Aumentar o tamanho do buffer
    while (1)
    {
        int bytes_recebidos = recv(sockfd, buffer, sizeof(buffer) - 1, 0); // Deixar espaço para o terminador nulo

        if (bytes_recebidos < 0)
        {
            printf("Erro ao receber mensagem");
            break;
        }
        else if (bytes_recebidos == 0)
        {
            // Conexão fechada pelo servidor
            printf("Conexão fechada pelo servidor\n");
            break;
        }
        else
        {
            buffer[bytes_recebidos] = '\0'; // Garantir que o buffer seja terminado com nulo

            // Dividir mensagens usando o delimitador '\n'
            char *mensagem = strtok(buffer, "\n");
            while (mensagem != NULL)
            {
                printf("\nServidor Respondeu: %s\n", mensagem);

                char log_msg[256];
                snprintf(log_msg, sizeof(log_msg), "Cliente ID: %d | Recebeu do servidor: %s", meuid, mensagem);
                escrevelog(log_msg, configCliente.ficheiro_log);

                if (strstr(mensagem, "kill") != NULL) // Usar strstr para verificar se "kill" está em qualquer parte da mensagem
                {
                    printf("Servidor mandou parar com isto tudo. Pressiona CTRL C\n");
                    terminado = 1;
                    break;
                }

                mensagem = strtok(NULL, "\n");
            }
        }
    }

    fecharsocket(&sockfd, configCliente, meuid);
    pthread_exit(NULL);
}

void handle_sigint(int sig)
{
    if (terminado == 1)
    {
        exit(0);
    }
    printf("\nRecebido sinal SIGINT (Ctrl+C). Saindo...\n");

    int bytes_enviados = send(sockfd, "kill", strlen("kill"), 0);
    if (bytes_enviados < 0)
    {
        perror("Erro ao enviar mensagem");
        exit(0);
    }
    else
    {
        printf("Fechando a ligação com o servidor\n");
    }

    exit(0); // Encerra o programa
}


void imprimemenu()
{
    int opcao = -1;
    printf("########## Bem-vindo ao Soduku (Cliente) ##########\n");
    printf("1 - Iniciar Jogo\n");
    printf("2 - Sair\n");
    printf("###################################################\n");
    while (opcao == -1)
    {
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpar o buffer de entrada para evitar problemas com fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        switch (opcao)
        {
        case 1:
            printf("Iniciar Jogo\n");
            criarsocket(&meuid, &sockfd, &serv_addr, hp,  configCliente);
            if (iniciarJogo(sockfd, configCliente, &jogo_iniciado, &trinco_jogo, meuid) > 0) {
                // logica restante
                //criar uma thread para escutar o servidor constantemente
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, RecebeInformacao, NULL);
                pthread_detach(thread_id);

                // ESTADO - EM JOGO
                //funcao para ler casas e enviar para o servidor
                printf("A enviar e escutar casas para enviar para o servidor\n");
                enviarEscutarCasasParaEnviarServidor(sockfd, &jogo_iniciado, &trinco_jogo, configCliente, meuid);

            } else {
                printf("Erro ao iniciar o jogo verifica as logs!\n");
                exit(1);
            }
            break;
        case 2:
            printf("Sair do Programa\n");
            exit(0);
            break;
        default:
            printf("Opção inválida\n");
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_sigint);
    if (argc != 2)
    {
        printf("Uso Correto: %s <ficheiro_configuracao>\n", argv[0]);
        return 1;
    }

    // Struct para os dados de config

    carregarConfigCliente(&configCliente, argv[1]);

    printf("++++++++++++ CONFIGURAÇÃO CARREGADA ++++++++++++ \n");
    printf("IP > %s\n", configCliente.ip);
    printf("Porta > %d\n", configCliente.porta);
    printf("Cliente ID > %d\n", configCliente.cliente_id);
    printf("Ficheiro LOG > %s\n", configCliente.ficheiro_log);
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++\n");

    // Apresentar Menu
    // enquanto nao for primido ctrl+c
    while (1)
    {
        imprimemenu();
    }
    return 0;
}