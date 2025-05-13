#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>
#include "handlers/print_jogo/printarjogo.h"
#include "handlers/valida_solucao/validarsolucao.h"
#include "handlers/log/escrevelogs.h"
#include "socket/socket.h"
#include "global.h"
#include <signal.h>
#include <time.h>
#include "handlers/statistics/stats.h"


ConfigServidor configServidor;

time_t start_time;
time_t end_time;
char *jogos; // GLOBAL VAR
char tabuleiro[82];
char tabuleiro_sol[82];
char **tab_sol;


//fazer um array de clientes que tem este formato [[ID,ganhos],[ID,ganhos]]
//cada cliente tem um id e um numero de jogos ganhos
int clientes_info[5000][2];
pthread_mutex_t trinco_clientes_array = PTHREAD_MUTEX_INITIALIZER;
//inicializar um array de pedidos
//mensagem_jogo é a struct que contem o pedido e o id do cliente
pthread_t pedidos[5000]; 
int nr_pedidos = 0;
int jogoterminou = 0;
//inicializar um trinco
pthread_mutex_t trinco_adicionarpedido = PTHREAD_MUTEX_INITIALIZER;
//trinco para escrita de logs em exclusao mutua ao ficheiro
pthread_mutex_t trinco_log = PTHREAD_MUTEX_INITIALIZER;



//MODO JOGO A
//TRINCO TABULEIRO A
pthread_mutex_t trinco_tabuleiro_A = PTHREAD_MUTEX_INITIALIZER;

//FIM MODO JOGO A
//canais de comunicacao abertos global array contem todos os socket ids
int client_sockets[5000];








//SEMÁFORO IMPARES
sem_t sem_impares;
pthread_mutex_t trinco_tabuleiro_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trinco_nr_pedidos_pares_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trinco_nr_pedidos_impares_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t trinco_par_impar_B = PTHREAD_MUTEX_INITIALIZER;
int numero_atual_pedidos_pares_B = 0;
int numero_atual_pedidos_impares_B = 0;
//FIM MODO JOGO B


//MODO JOGO C
sem_t sem__equipa_impares; //este sera inicializado a 1
sem_t sem__equipa_pares; //este sera inicializado a 0 pois não é a equipa que começa

pthread_mutex_t trinco_tabuleiro_C = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t jogadas_equipa_pares = PTHREAD_MUTEX_INITIALIZER;
int pontos_pares = 0;
int pontos_impares = 0;

//FIM MODO JOGO C

void handle_sigint() {
    printf("Recebido SIGINT, vou encerrar o servidor...\n");
    printf("Vou matar todos os clientes\n");
    broadcast_message("kill");
    exit(0);

}

void imprime_menu() {
    printf("########## Bem-vindo ao Soduku (Servidor) ##########\n");
    printf("1 - Iniciar Socket\n");
    printf("2 - Estatisticas\n");
    printf("3 - Sair\n");
    printf("####################################################\n");
    

}

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sigint);

    //inicializar tudo a -1
    srand(time(NULL));
    for (int i = 0; i < 5000; i++) {
        clientes_info[i][0] = -1;
        clientes_info[i][1] = -1;
    }


    //Inicialização semáforo dos impares MODO JOGO B
    sem_init(&sem_impares, 0, 0);

    //modo jogo C
    sem_init(&sem__equipa_impares,0,1);
    sem_init(&sem__equipa_pares,0,0);

    if (argc != 2) {
        fprintf(stderr, "Uso Correto: %s <ficheiro_configuracao>\n", argv[0]);
        return 1;
    }

    carregarConfigServidor(&configServidor, argv[1]);
    jogos = abrirficheiroJSON(configServidor.ficheiro_jogo, configServidor.ficheiro_log);
    tab_sol = obterarrayjogos(jogos, 1, configServidor.ficheiro_log);

    strncpy(tabuleiro, tab_sol[0], sizeof(tabuleiro) - 1);
    tabuleiro[sizeof(tabuleiro) - 1] = '\0';

    strncpy(tabuleiro_sol, tab_sol[1], sizeof(tabuleiro_sol) - 1);
    tabuleiro_sol[sizeof(tabuleiro_sol) - 1] = '\0';

    imprime_menu();

    int opcao = -1;
    while (1) {
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpar o buffer de entrada para evitar problemas com fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}

        switch (opcao) {
            case 1:
                printf("Iniciar Socket\n");
                int server_socket;
                struct sockaddr_in server_addr;

                setup_server(&server_socket, &server_addr, &configServidor);
                accept_clients(server_socket, &configServidor);

                close(server_socket);
                break;
            case 2:
                printf("Estatisticas\n");
                gerarstats();
                system("clear");
                imprime_menu();
                break;
            case 3:
                printf("Sair do Programa\n");
                exit(0);
                break;
            default:
                printf("Opção inválida\n");
                break;
        }
    }
    
    return 0;
}

