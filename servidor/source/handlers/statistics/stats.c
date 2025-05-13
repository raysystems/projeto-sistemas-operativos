#include "stats.h"


void gerarstats() {
    //ler o ficheiro de database/db.json
    int opcao = -1;
    FILE *ficheiro = fopen("database/db.json", "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir ficheiro");
        exit(1);
    }

    //obter o tamanho do ficheiro
    fseek(ficheiro, 0, SEEK_END);
    long length = ftell(ficheiro);
    fseek(ficheiro, 0, SEEK_SET);

    //alocar memoria para o conteudo do ficheiro
    char *content = malloc(length + 1);
    if (content == NULL) {
        perror("Erro ao alocar memoria");
        fclose(ficheiro);
        exit(1);
    }
    fread(content, 1, length, ficheiro);
    content[length] = '\0';
    fclose(ficheiro);

    //parse o conteudo do ficheiro
    cJSON *root = cJSON_Parse(content);
    free(content);
    if (root == NULL) {
        perror("Erro ao parsear JSON");
        exit(1);
    }

    //obter o array de jogos
    cJSON *jogos = cJSON_GetObjectItem(root, "jogos");
    if (jogos == NULL) {
        perror("Erro ao obter array de jogos");
        cJSON_Delete(root);
        exit(1);
    }

    //obter o numero de jogos
    int numero_jogos = cJSON_GetArraySize(jogos);

    //obter a media de jogo de cada modo de jogo A B C 
    int total_tempo_A = 0;
    int total_tempo_B = 0;
    int total_tempo_C = 0;

    int total_jogos_A = 0;
    int total_jogos_B = 0;
    int total_jogos_C = 0;

    for (int i = 0; i < numero_jogos; i++) {
        cJSON *jogo = cJSON_GetArrayItem(jogos, i);
        if (jogo == NULL) {
            continue;
        }
        cJSON *modo_item = cJSON_GetObjectItem(jogo, "gameMode");
        cJSON *tempo_item = cJSON_GetObjectItem(jogo, "tempo");
        if (modo_item == NULL || tempo_item == NULL) {
            continue;
        }
        char *modo = modo_item->valuestring;
        int tempo = tempo_item->valueint;

        if (strcmp(modo, "A") == 0) {
            total_tempo_A += tempo;
            total_jogos_A++;
        } else if (strcmp(modo, "B") == 0) {
            total_tempo_B += tempo;
            total_jogos_B++;
        } else if (strcmp(modo, "C") == 0) {
            total_tempo_C += tempo;
            total_jogos_C++;
        }
    }

    //calcular a media
    float media_A = total_jogos_A ? (float)total_tempo_A / total_jogos_A : 0;
    float media_B = total_jogos_B ? (float)total_tempo_B / total_jogos_B : 0;
    float media_C = total_jogos_C ? (float)total_tempo_C / total_jogos_C : 0;

    //limpar consola
    system("clear");

    //imprimir as estatisticas
    printf("###################### Estatisticas Gerais ######################\n");
    printf("Media de tempo de jogo do modo A: %.2f segundos\n", media_A);
    printf("Media de tempo de jogo do modo B: %.2f segundos\n", media_B);
    printf("Media de tempo de jogo do modo C: %.2f segundos\n", media_C);
    printf("#################################################################\n");

    opcao = -1;
    printf("\n");
    printf("########## Pagina 1/3 ##########\n");
    printf("\n");

    while (opcao == -1) {
        printf("1 - Continuar\n");
        printf("2 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpar o buffer de entrada para evitar problemas com fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        // se opcao for 1 sai do while
        if (opcao == 1) {
            break;
        }

        // se opcao for 2 imprime a pagina 3
        if (opcao == 2) {
            return;
        }
    }

    //para o A vamos mostrar o nr de jogos ganhos e perdidos e quem ganhou mais jogos o ID
    int total_ganhos_A = 0;
    int total_perdidos_A = 0;
    int id_mais_ganhos = -1;
    int max_ganhos = 0;
    int idsClientePontuacao[5000][2];
    //inicializar o array tudo a -1
    for (int i = 0; i < 5000; i++) {
        idsClientePontuacao[i][0] = -1;
        idsClientePontuacao[i][1] = 0;
    }

    for (int i = 0; i < numero_jogos; i++) {
        cJSON *jogo = cJSON_GetArrayItem(jogos, i);
        if (jogo == NULL) {
            continue;
        }
        cJSON *modo_item = cJSON_GetObjectItem(jogo, "gameMode");
        cJSON *vencedor_item = cJSON_GetObjectItem(jogo, "Winner");
        cJSON *id_item = cJSON_GetObjectItem(jogo, "gameId");
        
        if (modo_item == NULL || vencedor_item == NULL || id_item == NULL) {
            continue;
        }
        
        char *modo = modo_item->valuestring;
        char *vencedor = vencedor_item->valuestring;
        int gameId = id_item->valueint;
        int id = -1; // Inicializar com um valor inválido

        if (strcmp(modo, "A") == 0) {
            id = atoi(vencedor); // Converter vencedor para int
            
                total_ganhos_A++;
                //pesquisar ver se o winner ja esta no array
                int found = 0;
                for (int k = 0; k < 5000; k++) {
                    if (idsClientePontuacao[k][0] == id) {
                        idsClientePontuacao[k][1]++;
                        found = 1;
                        break;
                    }
                }
                //se nao esta vou inserir a entrada onde ha espaco
                if (!found) {
                    
                    for (int j = 0; j < 5000; j++) {
                        if (idsClientePontuacao[j][0] == -1) {
                            idsClientePontuacao[j][0] = id;
                            idsClientePontuacao[j][1] = 1;
                            break;
                        }
                    }
                } else {
                    
                }
            
        }
    }

    // Encontrar o ID com mais ganhos
    for (int i = 0; i < 5000; i++) {
        if (idsClientePontuacao[i][0] != -1 && idsClientePontuacao[i][1] > max_ganhos) {
            max_ganhos = idsClientePontuacao[i][1];
            id_mais_ganhos = idsClientePontuacao[i][0];
        }
    }

    //da print do array idsClientePontuacao
    for (int i = 0; i < 5000; i++) {
        if (idsClientePontuacao[i][0] != -1) {
            printf("ID: %d | Pontuacao: %d\n", idsClientePontuacao[i][0], idsClientePontuacao[i][1]);
        }
    }
    //limpar consola
    system("clear");
    // Exibir resultados
    printf("###################### Estatisticas Modo A ######################\n");
    printf("Total de jogos Modo A: %d\n", total_jogos_A);
    printf("ID com mais jogos ganhos no modo A: %d com %d vitórias\n", id_mais_ganhos, max_ganhos);
    printf("Media de Tempo: %.2f segundos\n", media_A);
    printf("#################################################################\n");

    opcao = -1;
    printf("\n");
    printf("########## Pagina 2/3 ##########\n");
    printf("\n");

    while (opcao == -1) {
        printf("1 - Continuar\n");
        printf("2 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpar o buffer de entrada para evitar problemas com fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        // se opcao for 1 sai do while
        if (opcao == 1) {
            break;
        }

        // se opcao for 2 imprime a pagina 3
        if (opcao == 2) {
            return;
        }
    }

    //Agora para o B que e por equipas Par e Impar vamos ver quem ganhou mais jogos e quem perdeu mais jogos
    int total_pares = 0;
    int total_impares = 0;

    for (int i = 0; i < numero_jogos; i++) {
        cJSON *jogo = cJSON_GetArrayItem(jogos, i);
        if (jogo == NULL) {
            continue;
        }
        cJSON *modo_item = cJSON_GetObjectItem(jogo, "gameMode");
        cJSON *vencedor_item = cJSON_GetObjectItem(jogo, "Winner");
        if (modo_item == NULL || vencedor_item == NULL) {
            continue;
        }
        char *modo = modo_item->valuestring;
        char *vencedor = vencedor_item->valuestring;
        int id = -1; // Inicializar com um valor inválido
        if (strcmp(modo, "B") == 0) {
            id = atoi(vencedor); // Converter vencedor para int
            if (id % 2 == 0) {
                total_pares++;
            } else if (id % 2 != 0) {
                total_impares++;
            }
        }
    }
    //limpar consola
    system("clear");
    printf("###################### Estatisticas Modo B ######################\n");
    //nr total de jogos ganhos por pares e impares
    int total = total_pares + total_impares;
    printf("Total de jogos Modo B: %d\n", total);   
    printf("Total de jogos ganhos por clientes com ID's Pares: %d\n", total_pares);
    printf("Total de jogos ganhos por clientes com ID's Impares: %d\n", total_impares);
    printf("Media de Tempo: %.2f segundos\n", media_B);
    printf("#################################################################\n");
    opcao = -1;
    printf("\n");
    printf("########## Pagina 3/3 ##########\n");
    printf("\n");

    while (opcao == -1) {
        printf("1 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        // Limpar o buffer de entrada para evitar problemas com fgets
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        // se opcao for 1 sai do while
        if (opcao == 1) {
            break;
        }

        // se opcao for 2 imprime a pagina 3
        if (opcao == 2) {
            return;
        }
    }
    // Liberar memória alocada para o objeto JSON
    cJSON_Delete(root);
}