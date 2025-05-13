#include "jogo.h"

int jogo_iniciado_animacao = 0;

void limparBufferStdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Remove caracteres residuais
}

void *loading_animation(void *arg) {
    const char *loading_chars = "|/-\\";
    int loading_index = 0;
    printf("Aguardando início do jogo ");
    fflush(stdout);
    while (jogo_iniciado_animacao == 0) {
        printf("\b%c", loading_chars[loading_index]);
        fflush(stdout);
        loading_index = (loading_index + 1) % 4;
        usleep(200000); // 200ms
    }
    return NULL;
}

int iniciarJogo(int sockfd, ConfigCliente configCliente , int *jogo_iniciado, pthread_mutex_t *trinco_jogo, int meuid) {

    //Primeiro envia a mensagem para o servidor que quer jogar

    const char *mensagem = "jogar";

    // Enviar mensagem para o socket
    int bytes_enviados = send(sockfd, mensagem, strlen(mensagem), 0);

    if (bytes_enviados < 0) {
        perror("Erro ao enviar mensagem");
        escrevelog("Erro ao enviar mensagem para iniciar o jogo. Mensagem: jogar", configCliente.ficheiro_log);
    } else {
        //printf("\033[H\033[J");
        printf("########################\n");
        printf("Mensagem enviada para o servidor para iniciar o jogo\n");
        printf("Assim que se reunir as condições necessárias o jogo irá começar!\n");
        printf("Nota: Para enviares casas deves seguir o formato:\n");
        printf("linha:coluna:valor\n");
        printf("Exemplo: 1:1:5\n");
        printf("Se quiseres mandar mais que uma segue o mesmo formato só que separa as casas por vírgulas!\n");
        printf("Exemplo: 1:1:5,1:2:6,1:3:7\n");
        printf("########################\n");

        // Criar a thread para a animação de loading
        pthread_t loading_thread;
        pthread_create(&loading_thread, NULL, loading_animation, NULL);

        char buffer[1024];
        int bytes_recebidos = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_recebidos > 0) {
            buffer[bytes_recebidos] = '\0';
            //printf("\033[H\033[J");
           
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Cliente ID: %d | Recebeu do servidor: %s", meuid, buffer);
            escrevelog(log_msg, configCliente.ficheiro_log);
          
            //printf("\nResposta do servidor: %s\n", buffer);
            if (strcmp(buffer, "comecou") == 0) {
                pthread_mutex_lock(trinco_jogo);
                *jogo_iniciado = 1;
                pthread_mutex_unlock(trinco_jogo);
                jogo_iniciado_animacao = 1;
            } 
        } else {
            printf("Erro ao receber mensagem do servidor\n");
            escrevelog("Erro ao receber mensagem do servidor", configCliente.ficheiro_log);
            pthread_cancel(loading_thread);
            pthread_join(loading_thread, NULL);
            return -1;
        }

        pthread_cancel(loading_thread);
        pthread_join(loading_thread, NULL);

        //Apos receber a mensagem do servidor
        printf("A iniciar o jogo...\n");
        return 1;
    }




}

void enviarEscutarCasasParaEnviarServidor(int sockfd, int *estadojogo, pthread_mutex_t *trinco_jogo, ConfigCliente configCliente, int meuid) {
    // ESTADO - EM JOGO
    printf("Nota: Para enviares casas deves seguir o formato:\n");
    printf("linha:coluna:valor\n");
    printf("Exemplo: 1:1:5\n");
    printf("Se quiseres mandar mais que uma segue o mesmo formato só que separa as casas por vírgulas!\n");
    printf("Exemplo: 1:1:5,1:2:6,1:3:7\n");

    while (1) {
        pthread_mutex_lock(trinco_jogo);
        if (*estadojogo == 0) {
            pthread_mutex_unlock(trinco_jogo);
            break;
        }
        pthread_mutex_unlock(trinco_jogo);

        // Ficar a ler inputs teclado
        char buffer[1024];
        printf("Escreva a sua jogada: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Erro ao ler entrada. Tente novamente.\n");
            limparBufferStdin(); // Limpa buffer no caso de erro
            continue;
        }

        // Remover o caractere de nova linha, se presente
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verificar se o buffer está vazio
        if (strlen(buffer) == 0) {
            printf("Erro: Mensagem vazia! Tente novamente\n");
            continue;
        }

        // Verificar se cumpre o padrão de envio
        int formato_valido = 1;
        char buffer_copy[1024];
        strncpy(buffer_copy, buffer, sizeof(buffer_copy));
        buffer_copy[sizeof(buffer_copy) - 1] = '\0'; // Garantir que está terminado em null

        char *token = strtok(buffer_copy, ",");
        while (token != NULL) {
            char *linha = strtok(token, ":");
            char *coluna = strtok(NULL, ":");
            char *valor = strtok(NULL, ":");
            if (linha == NULL || coluna == NULL || valor == NULL) {
                printf("Erro: Formato inválido! Tente novamente\n");
                formato_valido = 0;
                break;
            }

            int linha_int = atoi(linha);
            int coluna_int = atoi(coluna);
            int valor_int = atoi(valor);

            if (linha_int < 1 || linha_int > 9 || coluna_int < 1 || coluna_int > 9 || valor_int < 1 || valor_int > 9) {
                printf("Erro: Linha, coluna e valor devem estar entre 1 e 9! Tente novamente\n");
                formato_valido = 0;
                break;
            }

            token = strtok(NULL, ",");
        }

        if (!formato_valido) {
            continue;
        }

        // Enviar mensagem para o socket
        int bytes_enviados = send(sockfd, buffer, strlen(buffer), 0);
        if (bytes_enviados < 0) {
            perror("Erro ao enviar mensagem");
            break;
        } else {
            printf("Mensagem enviada: %s\n", buffer);

            // Registrar no log
            char log_msg[256];
            snprintf(log_msg, sizeof(log_msg), "Cliente ID: %d | Enviei para o servidor a(s) casa(s): %s", meuid, buffer);
            escrevelog(log_msg, configCliente.ficheiro_log);
        }
    }
}