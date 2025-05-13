#ifndef GLOBAL_H
#define GLOBAL_H

extern char tabuleiro[82];
extern char tabuleiro_sol[82];
#include "handlers/cfg_reader/configreader.h"
#include <semaphore.h>
extern ConfigServidor configServidor;

extern int client_sockets[5000];
extern time_t start_time;
extern time_t end_time;
extern int clientes_info[5000][2];
extern pthread_mutex_t trinco_clientes_array;

typedef struct {
    char casas[3];
    int idcliente;
    int sockfd;

} mensagem_jogo;
extern int nr_pedidos;
extern int jogoterminou;
extern pthread_mutex_t trinco_adicionarpedido;
extern pthread_mutex_t trinco_tabuleiro_A;
extern pthread_t pedidos[5000]; 

//modo jogo B
extern pthread_mutex_t trinco_nr_pedidos_pares_B;
extern pthread_mutex_t trinco_nr_pedidos_impares_B;
extern pthread_mutex_t trinco_par_impar_B;
extern pthread_mutex_t trinco_tabuleiro_B;
extern sem_t sem_impares;
extern int numero_atual_pedidos_pares_B;
extern int numero_atual_pedidos_impares_B;
//Fim modo jogo B


//MODO JOGO C
extern sem_t sem__equipa_impares;
extern sem_t sem__equipa_pares;
extern pthread_mutex_t trinco_tabuleiro_C;
pthread_mutex_t jogadas_equipa_pares ;
extern int pontos_pares;
extern int pontos_impares;
//FIM MODO JOGO C

extern pthread_mutex_t trinco_log;

#endif // GLOBAL_H