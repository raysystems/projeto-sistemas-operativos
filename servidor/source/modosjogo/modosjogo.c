//definir uma estrutura global neste caso vai ser o tabuleiro de jogo
//array para 81 casas

#include "modosjogo.h"
#include <unistd.h>


void * processarPedido_A(void *arg) {
    mensagem_jogo *pedido = (mensagem_jogo *)arg;
    //obter o socket da estrutura
    int sockfd = pedido->sockfd;
    //obter o id do cliente
    int clientID = pedido->idcliente;
    //obter as casas
    char *casas = pedido->casas;

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Thread ID %lu Criada pelo ClienteID %d Vou tratar de verificar a casa %d:%d com o valor %c", (unsigned long)pthread_self(),clientID, casas[0], casas[1], casas[2]);
    escrevelog(log_msg, configServidor.ficheiro_log);
    
    sleep(8);
    if (tabuleiro_sol[(casas[0] - 1) * 9 + (casas[1] - 1)] == casas[2]) {
        //a casa é correta tudo bem mas temos que validar se esta preenchida por alguem
        //se estiver preenchida por alguem nao podemos aceitar
        //trinco
        pthread_mutex_lock(&trinco_tabuleiro_A);
        
        if (tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] != '0') {
        
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d preenchida por alguem \n",(unsigned long)pthread_self(),clientID, casas[0], casas[1]);
            escrevelog(log_msg, configServidor.ficheiro_log);
            //enviar mensagem ao cliente
            char msg[256];
            snprintf(msg, sizeof(msg), "Casa %d:%d ja preenchida por alguem. Este ClienteID: %d tentou preencher! Valor: %c \n", casas[0], casas[1],clientID, casas[2]);
            send(sockfd, msg, strlen(msg), 0);
            pthread_mutex_unlock(&trinco_tabuleiro_A);
            free(pedido); // liberar a memória alocada
            pthread_exit(NULL);
        } else {
            //casa correta e nao preenchida por ninguem
            //vamos preencher a casa
            tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] = casas[2];
            //enviar mensagem ao cliente
            char msg[256];
            enviarParaEcra(tabuleiro);
            snprintf(msg, sizeof(msg), "Casa %d:%d preenchida com sucesso. ClienteID: %d Valor: %c \n", casas[0], casas[1],clientID,  casas[2]);
            pthread_mutex_lock(&trinco_clientes_array);
            pontuar_cliente(clientID);
            pthread_mutex_unlock(&trinco_clientes_array);
            escrevelog(msg, configServidor.ficheiro_log);
            send(sockfd, msg, strlen(msg), 0);

            //depois ver se o jogo terminou
            if (validarjogo(tabuleiro) == 0 && jogoterminou == 0) {
                //jogo terminou
                char msg[256];
                snprintf(msg, sizeof(msg), "Jogo terminou. O tabuleiro está completo: %d \n", clientID);
                escrevelog(msg, configServidor.ficheiro_log);
                send(sockfd, msg, strlen(msg), 0);
                jogoterminou = 1;
                //enviar mensagem a todos os clientes
                broadcast_message("JA EXISTE VENCEDOR\n");
                broadcast_message("kill\n");
                //detectar o vencedor
                int id_vencedor = analizar_cliente_mais_pontuado();
                // id_vencedor converter para char*
                char id_vencedor_str[256];
                end_time = time(NULL);
                snprintf(id_vencedor_str, sizeof(id_vencedor_str), "%d", id_vencedor);
                guardarJogo('A', id_vencedor_str);


            }

        }
        pthread_mutex_unlock(&trinco_tabuleiro_A);
     
    } else {
        //casa incorreta

        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d com valor %c incorreto",(unsigned long)pthread_self(),clientID, casas[0], casas[1], casas[2]);
        escrevelog(log_msg, configServidor.ficheiro_log);
        //enviar mensagem ao cliente
        pthread_mutex_lock(&trinco_clientes_array);
        despontuar_cliente(clientID);
        pthread_mutex_unlock(&trinco_clientes_array);
        char msg[256];
        snprintf(msg, sizeof(msg), "Casa %d:%d com valor %c incorreto\n", casas[0], casas[1], casas[2]);
        send(sockfd, msg, strlen(msg), 0);
    }

    
    free(pedido); // liberar a memória alocada
    printf("Pedido processado\n");
    pthread_exit(NULL);
}

void modojogoA(char *buffer, int clientID, int sockfd, ConfigServidor configServidor) {

    //vamos criar um pedido para cada casa recebida no buffer
    //relembro que cada casa vem neste forato linha:coluna:valor e se vier multiplas casas
    //vem separadas por , exemplo 1:1:1,2:2:2,3:3:3
    //vamos criar um pedido para cada casa

    char *casa = strtok(buffer, ",");
    while (casa != NULL) {
        //criar um pedido
        mensagem_jogo *pedido = malloc(sizeof(mensagem_jogo));
        //preencher o pedido
        sscanf(casa, "%d:%d:%c", &pedido->casas[0], &pedido->casas[1], &pedido->casas[2]);
        pedido->idcliente = clientID;
        pedido->sockfd = sockfd;
        
        //para cada pedido vamos criar uma thread para processar o pedido
        //a thread vai ser responsavel por verificar se a casa esta correta
        //criar uma thread e adicionar ao array de threads
        pthread_t tratar_pdd;
        pthread_create(&tratar_pdd, NULL, processarPedido_A, (void *)pedido);
        pthread_detach(tratar_pdd);

        casa = strtok(NULL, ",");
    }
}

void *processarPedido_B(void *arg){
    mensagem_jogo *pedido = (mensagem_jogo *)arg;
    //obter o socket da estrutura
    int sockfd = pedido->sockfd;
    //obter o id do cliente
    int clientID = pedido->idcliente;
    //obter as casas
    char *casas = pedido->casas;

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Thread ID %lu Criada pelo ClienteID %d Vou tratar de verificar a casa %d:%d com o valor %c", (unsigned long)pthread_self(),clientID, casas[0], casas[1], casas[2]);
    escrevelog(log_msg, configServidor.ficheiro_log);
    //vamos ver se somos par ou impar se par incrementar o numero de pedidos pares
    
    pthread_mutex_lock(&trinco_par_impar_B);
    printf("THREAD ID %lu Estou no trinco_par_impar_B %d\n", (unsigned long)pthread_self());
    if (clientID % 2 == 0) {
    // Sou par, incremento número de pedidos pares
    //pthread_mutex_lock(&trinco_nr_pedidos_pares_B);
        numero_atual_pedidos_pares_B++;
    //pthread_mutex_unlock(&trinco_nr_pedidos_pares_B);
    }

    // Vamos ver se temos mais de 2 pedidos em curso
    
    if (numero_atual_pedidos_pares_B > 0 && clientID % 2 != 0) {
        // Sou ímpar e há pares em processamento, preciso esperar
        //pthread_mutex_unlock(&trinco_nr_pedidos_pares_B);

        // Incremento número de pedidos ímpares
        //pthread_mutex_lock(&trinco_nr_pedidos_impares_B);

        numero_atual_pedidos_impares_B++;
        printf("Numero de pedidos impares %d\n", numero_atual_pedidos_impares_B);
        //pthread_mutex_unlock(&trinco_nr_pedidos_impares_B);

        // Log de espera
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Thread ID %lu a espera que os pares terminem! Thread Pares em validacao %d.", 
                (unsigned long)pthread_self(), numero_atual_pedidos_pares_B);
        escrevelog(log_msg, configServidor.ficheiro_log);

        // Espera no semáforo dos ímpares
        
        pthread_mutex_unlock(&trinco_par_impar_B);
        printf("THREAD ID %lu Sai do  155 trinco_par_impar_B %d\n", (unsigned long)pthread_self());
        
        sem_wait(&sem_impares);
        
        pthread_mutex_lock(&trinco_par_impar_B);
        if (numero_atual_pedidos_impares_B > 0) {
            numero_atual_pedidos_impares_B--;
        } else {
            numero_atual_pedidos_impares_B = 0;
        }
        pthread_mutex_unlock(&trinco_par_impar_B);
    } 
    pthread_mutex_unlock(&trinco_par_impar_B);
    printf("THREAD ID %lu  Sai do  161 no trinco_par_impar_B %d\n", (unsigned long)pthread_self());

    //vamos ver se tem mais de dois pedidos em curso
    sleep(8);
    if (tabuleiro_sol[(casas[0] - 1) * 9 + (casas[1] - 1)] == casas[2]) {
        //a casa é correta tudo bem mas temos que validar se esta preenchida por alguem
        //se estiver preenchida por alguem nao podemos aceitar
        //trinco (no tabuleiro)

        pthread_mutex_lock(&trinco_tabuleiro_B);
        printf("THREAD ID %lu  trinco_tabuleiro_B %d\n", (unsigned long)pthread_self());
        
        if (tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] != '0') {
            
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d esta correto mas esta preenchida por alguem",(unsigned long)pthread_self() ,clientID, casas[0], casas[1]);
            escrevelog(log_msg, configServidor.ficheiro_log);
            //enviar mensagem ao cliente
            char msg[256];
            snprintf(msg, sizeof(msg), "Casa %d:%d esta correta mas ja esta preenchida por alguem. Este ClienteID: %d tentou preencher! Valor: %c \n", casas[0], casas[1],clientID, casas[2]);
            send(sockfd, msg, strlen(msg), 0);
            pthread_mutex_unlock(&trinco_tabuleiro_B);
            printf("THREAD ID %lu SAI 183 trinco_tabuleiro_B %d\n", (unsigned long)pthread_self());
            //cenario em que a casa esta preenchida por alguem 
            //vamos decrementar o numero de pedidos pares ou impares consoante caso
            pthread_mutex_lock(&trinco_par_impar_B);
            
            if (clientID % 2 == 0) {
                
                
                if (numero_atual_pedidos_pares_B > 0) {
                    numero_atual_pedidos_pares_B--;
                } else {
                    numero_atual_pedidos_pares_B = 0;
                }
               
            } 
            
            if (numero_atual_pedidos_pares_B == 0 && clientID % 2 == 0) {
                
                char log_msg[256];
                snprintf(log_msg, sizeof(log_msg), "Todos os pares acabaram as validações então libertei %d Impares.", numero_atual_pedidos_pares_B);
                escrevelog(log_msg, configServidor.ficheiro_log);
                
                for (int i = 0; i < numero_atual_pedidos_impares_B; i++) {
                    sem_post(&sem_impares);
                }
                

            }
            pthread_mutex_unlock(&trinco_par_impar_B);
            printf("THREAD ID %lu 211 trinco_par_impar_B %d\n", (unsigned long)pthread_self());


            free(pedido); // liberar a memória alocada
            pthread_exit(NULL);
        } else {
            //casa correta e nao preenchida por ninguem
            //vamos preencher a casa
            tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] = casas[2];
            //enviar mensagem ao cliente
            char msg[256];
            enviarParaEcra(tabuleiro);
            snprintf(msg, sizeof(msg), "Casa %d:%d preenchida com sucesso. ClienteID: %d Valor: %c \n", casas[0], casas[1],clientID,  casas[2]);
            pontuar_cliente(clientID);
            escrevelog(msg, configServidor.ficheiro_log);
            send(sockfd, msg, strlen(msg), 0);

            //depois ver se o jogo terminou
            if (validarjogo(tabuleiro) == 0 && jogoterminou == 0) {
                //jogo terminou
                char msg[256];
                snprintf(msg, sizeof(msg), "Jogo terminou. O tabuleiro está completo: %d \n", clientID);
                escrevelog(msg, configServidor.ficheiro_log);
                send(sockfd, msg, strlen(msg), 0);
                jogoterminou = 1;
                //enviar mensagem a todos os clientes
                broadcast_message("JA EXISTE VENCEDOR \n");
                broadcast_message("kill\n");
                //detectar o vencedor
                int id_vencedor = analizar_cliente_mais_pontuado();
                // id_vencedor converter para char*
                char id_vencedor_str[256];
                end_time = time(NULL);
                snprintf(id_vencedor_str, sizeof(id_vencedor_str), "%d", id_vencedor);
                guardarJogo('B', id_vencedor_str);


            } 
            pthread_mutex_unlock(&trinco_tabuleiro_B);
            printf("THREAD ID %lu 251 trinco_tabuleiro_B %d\n", (unsigned long)pthread_self());

        }
        
     
    } else {
        //casa incorreta
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d com valor %c incorreto",(unsigned long)pthread_self(),clientID, casas[0], casas[1], casas[2]);
        escrevelog(log_msg, configServidor.ficheiro_log);
        //enviar mensagem ao cliente
        char msg[256];
        snprintf(msg, sizeof(msg), "Casa %d:%d com valor %c incorreto \n", casas[0], casas[1], casas[2]);
        pthread_mutex_lock(&trinco_clientes_array);
        despontuar_cliente(clientID);
        pthread_mutex_unlock(&trinco_clientes_array);
        send(sockfd, msg, strlen(msg), 0);

    }

    //vamos decrementar o numero de pedidos pares ou impares consoante caso
    pthread_mutex_lock(&trinco_par_impar_B);
    printf("THREAD ID %lu 267 trinco_tabuleiro_B %d\n", (unsigned long)pthread_self());
    if (clientID % 2 == 0) {
        if (numero_atual_pedidos_pares_B > 0) {
            numero_atual_pedidos_pares_B--;
        } else {
            numero_atual_pedidos_pares_B = 0;
        }
        
    } 

    //ver se o ultimo par passou se sim for para fazer post aos impares
   
    if (numero_atual_pedidos_pares_B == 0 && clientID % 2 == 0 && numero_atual_pedidos_impares_B > 0) {
        //log
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), "Todos os pares acabaram as validações então libertei %d Impares.", numero_atual_pedidos_impares_B);
        escrevelog(log_msg, configServidor.ficheiro_log);
        for (int i = 0; i < numero_atual_pedidos_impares_B; i++) {

            sem_post(&sem_impares);
        }
        
    } 
    pthread_mutex_unlock(&trinco_par_impar_B);  
    printf("THREAD ID %lu 289 trinco_tabuleiro_B %d\n", (unsigned long)pthread_self());
    free(pedido); // liberar a memória alocada
    printf("Pedido processado\n");
    pthread_exit(NULL);
}

//neste modo de jogo os clientIDs pares têm prioridade sobre os ímpares
//e se estiver a chegar sempre clientIDs pares, os ímpares nunca vão conseguir jogar
//NÃO É PARA TRATAR DESSE PROBLEMA
//o trinco vai ser no tabuleiro
void modojogoB(char *buffer, int clientID, int sockfd, ConfigServidor configServidor) {
    printf("Modo de jogo B\n");

    char *casa = strtok(buffer, ",");
    //criar um pedido por casa
    while (casa != NULL) {
        //criar um pedido
        mensagem_jogo *pedido = malloc(sizeof(mensagem_jogo));
        //preencher o pedido
        sscanf(casa, "%d:%d:%c", &pedido->casas[0], &pedido->casas[1], &pedido->casas[2]);
        pedido->idcliente = clientID;
        pedido->sockfd = sockfd;
        
        //para cada pedido vamos criar uma thread para processar o pedido
        //a thread vai ser responsavel por verificar se a casa esta correta
        //criar uma thread e adicionar ao array de threads
        pthread_t tratar_pdd;
        pthread_create(&tratar_pdd, NULL, processarPedido_B, (void *)pedido);
        pthread_detach(tratar_pdd);

        casa = strtok(NULL, ",");
    }
}

void *processarPedido_C(void *arg) {
    mensagem_jogo *pedido = (mensagem_jogo *)arg;
    int sockfd = pedido->sockfd;
    int clientID = pedido->idcliente;
    char *casas = pedido->casas;

    char log_msg[256];
    snprintf(log_msg, sizeof(log_msg), "Thread ID %lu Criada pelo ClienteID %d Vou tratar de verificar a casa %d:%d com o valor %c", (unsigned long)pthread_self(), clientID, casas[0], casas[1], casas[2]);
    escrevelog(log_msg, configServidor.ficheiro_log);

    if (clientID % 2 == 0) {
        // Sou par, fica inicialmente à espera pois quem joga primeiro é um ímpar 
        sem_wait(&sem__equipa_pares);
    } else {
        // Sou ímpar, inicialmente passa 1
        sem_wait(&sem__equipa_impares);
    }

    sleep(8); // Simula o tempo de processamento

    pthread_mutex_lock(&trinco_tabuleiro_C);
    if (tabuleiro_sol[(casas[0] - 1) * 9 + (casas[1] - 1)] == casas[2]) {
        if (tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] != '0') {
            snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d esta correto mas esta preenchida por alguem", (unsigned long)pthread_self(), clientID, casas[0], casas[1]);
            escrevelog(log_msg, configServidor.ficheiro_log);

            char msg[256];
            snprintf(msg, sizeof(msg), "Casa %d:%d esta correta mas ja esta preenchida por alguem. Este ClienteID: %d tentou preencher! Valor: %c \n", casas[0], casas[1], clientID, casas[2]);
            send(sockfd, msg, strlen(msg), 0);
        } else {
            tabuleiro[(casas[0] - 1) * 9 + (casas[1] - 1)] = casas[2];
            pontuar_cliente(clientID);
            enviarParaEcra(tabuleiro);

            snprintf(log_msg, sizeof(log_msg), "Casa %d:%d preenchida com sucesso. ClienteID: %d Valor: %c", casas[0], casas[1], clientID, casas[2]);
            escrevelog(log_msg, configServidor.ficheiro_log);

            char msg[256];
            snprintf(msg, sizeof(msg), "Casa %d:%d preenchida com sucesso. ClienteID: %d Valor: %c \n", casas[0], casas[1], clientID, casas[2]);
            send(sockfd, msg, strlen(msg), 0);

            if (validarjogo(tabuleiro) == 0 && jogoterminou == 0) {
                snprintf(log_msg, sizeof(log_msg), "Jogo terminou. O tabuleiro está completo: %d \n", clientID);
                escrevelog(log_msg, configServidor.ficheiro_log);
                send(sockfd, log_msg, strlen(log_msg), 0);
                jogoterminou = 1;
                end_time = time(NULL);

                
                if (clientID % 2 == 0) {
                    pontos_pares++;
                } else {
                    pontos_impares++;
                }

                if (pontos_pares > pontos_impares) {
                    guardarJogo('C', "Pares");
                    broadcast_message("Pares venceram\n");
                } else if (pontos_pares < pontos_impares) {
                    guardarJogo('C', "Impares");
                    broadcast_message("Impares venceram\n");
                } else {
                    guardarJogo('C', "Empate");
                    broadcast_message("Empate\n");
                }

                broadcast_message("JA EXISTE VENCEDOR\n");
                broadcast_message("kill\n");
            }

        }
    } else {
        snprintf(log_msg, sizeof(log_msg), "Thread ID %lu | Cliente ID %d enviou a Casa %d:%d com valor %c incorreto", (unsigned long)pthread_self(), clientID, casas[0], casas[1], casas[2]);
        escrevelog(log_msg, configServidor.ficheiro_log);
        if (clientID % 2 == 0) {
            if (pontos_pares > 0) {
                pontos_pares--;
            } else {
                pontos_pares = 0;
            }
            
        } else {
            if (pontos_impares > 0) {
                pontos_impares--;
            } else {
                pontos_impares = 0;
            }
            
        }
        char msg[256];
        snprintf(msg, sizeof(msg), "Casa %d:%d com valor %c incorreto \n", casas[0], casas[1], casas[2]);
        send(sockfd, msg, strlen(msg), 0);
    }
    pthread_mutex_unlock(&trinco_tabuleiro_C);

    if (clientID % 2 == 0) {
        broadcast_message("Agora quem joga e os impares \n");
        sem_post(&sem__equipa_impares);
    } else {
        broadcast_message("Agora quem joga e os pares \n");
        sem_post(&sem__equipa_pares);
    }

    free(pedido);
    pthread_exit(NULL);
}

void modojogoC(char *buffer, int clientID, int sockfd, ConfigServidor configServidor) {
    printf("Modo de jogo C\n");

    char *casa = strtok(buffer, ",");
    while (casa != NULL) {
        //criar um pedido
        mensagem_jogo *pedido = malloc(sizeof(mensagem_jogo));
        //preencher o pedido
        sscanf(casa, "%d:%d:%c", &pedido->casas[0], &pedido->casas[1], &pedido->casas[2]);
        pedido->idcliente = clientID;
        pedido->sockfd = sockfd;
        
        //para cada pedido vamos criar uma thread para processar o pedido
        //a thread vai ser responsavel por verificar se a casa esta correta
        //criar uma thread e adicionar ao array de threads
        pthread_t tratar_pdd;
        pthread_create(&tratar_pdd, NULL, processarPedido_C, (void *)pedido);
        pthread_detach(tratar_pdd);

        casa = strtok(NULL, ",");
    }
}

void modojogoD() {
    printf("Modo de jogo D\n");
}

void modojogoE() {
    printf("Modo de jogo E\n");
}


void guardarJogo(char modo, char *vencedor) {
    FILE *ficheiro = fopen("database/db.json", "r");
    cJSON *root;
    cJSON *jogos;

    if (ficheiro != NULL) {
        // Lê o conteúdo existente do arquivo
        fseek(ficheiro, 0, SEEK_END);
        long length = ftell(ficheiro);
        fseek(ficheiro, 0, SEEK_SET);
        char *content = malloc(length + 1);
        fread(content, 1, length, ficheiro);
        content[length] = '\0';
        fclose(ficheiro);

        // Parseia o conteúdo JSON existente
        root = cJSON_Parse(content);
        free(content);
        if (root == NULL) {
            root = cJSON_CreateObject();
        }
        jogos = cJSON_GetObjectItem(root, "jogos");
        if (jogos == NULL) {
            jogos = cJSON_CreateArray();
            cJSON_AddItemToObject(root, "jogos", jogos);
        }
    } else {
        // Cria um novo objeto JSON se o arquivo não existir
        root = cJSON_CreateObject();
        jogos = cJSON_CreateArray();
        cJSON_AddItemToObject(root, "jogos", jogos);
    }

    // Adiciona um novo jogo ao array
    cJSON *jogo = cJSON_CreateObject();
    cJSON_AddItemToArray(jogos, jogo);

    // Adiciona os campos ao jogo
    cJSON_AddNumberToObject(jogo, "gameId", rand());
    // Adiciona a data do jogo
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char gameDate[20];
    sprintf(gameDate, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    cJSON_AddStringToObject(jogo, "gameDate", gameDate);

    // Corrige a passagem do modo de jogo
    char modo_str[2] = {modo, '\0'};
    cJSON_AddStringToObject(jogo, "gameMode", modo_str);

    // Adiciona o vencedor
    cJSON_AddStringToObject(jogo, "Winner", vencedor);

    // Calcula o tempo decorrido
    time_t tempo = end_time - start_time;
    cJSON_AddNumberToObject(jogo, "tempo", tempo);

    // Converte o objeto JSON para string
    char *json_string = cJSON_Print(root);

    // Abre o arquivo em modo de escrita
    ficheiro = fopen("database/db.json", "w");
    if (ficheiro == NULL) {
        perror("Erro ao abrir ficheiro");
        exit(1);
    }

    // Escreve a string JSON no ficheiro
    fprintf(ficheiro, "%s", json_string);

    // Libera a memória
    free(json_string);
    cJSON_Delete(root);
    fclose(ficheiro);
}

void pontuar_cliente(int id) {
    //vamos procurar primeiro a ver se existe o cliente no array de clientes

    for (int i = 0; i < 5000; i++) {
        if (clientes_info[i][0] == id) {
            //cliente encontrado
            clientes_info[i][1] += 1;
            return;
        }
    }
    //cliente nao encontrado
    //vamos colocar na primeira posicao livre
    for (int i = 0; i < 5000; i++) {
        if (clientes_info[i][0] == -1) {
            clientes_info[i][0] = id;
            clientes_info[i][1] = 1;
            return;
        }
    }
 

}

void despontuar_cliente(int id) {

    for (int i = 0; i < 5000; i++) {
        if (clientes_info[i][0] == id) {
            if (clientes_info[i][1] > 0) {

                clientes_info[i][1] -= 1;
            }	

            return;
        }
    }

}

int analizar_cliente_mais_pontuado() {
    int max = 0;
    int id = -1;
    pthread_mutex_lock(&trinco_clientes_array);
    for (int i = 0; i < 5000; i++) {
        if (clientes_info[i][0] != -1 && clientes_info[i][1] > max) {
            max = clientes_info[i][1];
            id = clientes_info[i][0];
        }
    }
    pthread_mutex_unlock(&trinco_clientes_array);

    return id;

}

void retirar_cliente_pontuacao(int id) {
    pthread_mutex_lock(&trinco_clientes_array);
    for (int i = 0; i < 5000; i++) {
        if (clientes_info[i][0] == id) {
            clientes_info[i][0] = -1;
            clientes_info[i][1] = -1;
            return;
        }
    }
    pthread_mutex_unlock(&trinco_clientes_array);
}

