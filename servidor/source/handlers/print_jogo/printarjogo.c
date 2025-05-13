
#include "printarjogo.h"
char* abrirficheiroJSON(const char* caminhoFicheiro, char* logs_nomeficheiro) {

    FILE *fp = fopen(caminhoFicheiro, "r");  
    if (fp == NULL) {
        escrevelog("Erro ao abrir o ficheiro JSON dos jogos", logs_nomeficheiro);
        return NULL;  
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);  
    fseek(fp, 0, SEEK_SET);   

    char* conteudo = (char*)malloc(fsize + 1);  
    if (conteudo == NULL) {  
        fclose(fp);  
        escrevelog("Erro ao alocar memória para o conteúdo do ficheiro JSON", logs_nomeficheiro);
        return NULL;
    }

    fread(conteudo, 1, fsize, fp);  
    conteudo[fsize] = '\0';  

    fclose(fp);  
    escrevelog("Leitura dos jogos efetuada do ficheiro", logs_nomeficheiro);  
    return conteudo;  
}


char** obterarrayjogos(const char* json, int idjogoProcurado, char* logs_nomeficheiro) {
    cJSON *jogosJson = cJSON_Parse(json); 
    if (jogosJson == NULL) {
        escrevelog("Erro ao fazer parse do JSON, ficheiro dos jogos.json", logs_nomeficheiro);
        return NULL; // Deve retornar NULL em vez de void
    }

    cJSON *jogoArray = cJSON_GetObjectItem(jogosJson, "jogos");
    if (!cJSON_IsArray(jogoArray)) {
        escrevelog("Erro ao obter o array de jogos", logs_nomeficheiro);
        cJSON_Delete(jogosJson);
        return NULL;
    }

    int tamanho = cJSON_GetArraySize(jogoArray);
    for (int i = 0; i < tamanho; i++) {
        cJSON *jogo = cJSON_GetArrayItem(jogoArray, i);
        cJSON *id = cJSON_GetObjectItem(jogo, "idJogo");

        if (id->valueint == idjogoProcurado) {
            // Obter tabuleiro e solução
            cJSON *tabuleiro = cJSON_GetObjectItem(jogo, "tabuleiroProposto");
            cJSON *solucao = cJSON_GetObjectItem(jogo, "solucao");

            if (!cJSON_IsString(tabuleiro) || !cJSON_IsString(solucao)) {
                escrevelog("Erro ao obter o tabuleiro ou a solução", logs_nomeficheiro);
                cJSON_Delete(jogosJson);
                return NULL;
            }

            // Alocar memória para o array de retorno
            char **array = (char**)malloc(2 * sizeof(char*));
            array[0] = strdup(tabuleiro->valuestring); // Usar strdup para duplicar a string
            array[1] = strdup(solucao->valuestring);   

            cJSON_Delete(jogosJson);
            return array; 
        }
    }

    cJSON_Delete(jogosJson);
    return NULL; 
}

//0 - jogo || 1 - solucao
void enviarParaEcra(char* tab_sol) {
    int barra = 0;
    int novalinha = 0;
    int novalinhatracos = 0;
    int iniciolinha = 0;
    printf("############### Tabuleiro ############### \n");
    printf(" | ---------------------- | \n");
     printf(" | ");
    for(int i = 0; i < 81; i++) {
        if (novalinha == 9) {
            novalinhatracos++;
            printf(" | ");
            printf("\n");
            printf(" | ");
            
            novalinha = 0;
            barra = 0;
        }

        if (novalinhatracos == 3) {
           
            printf("---------------------- |  \n");
            printf(" | ");
            novalinhatracos = 0;
        }

        if (barra == 3) {
            printf("| ");
            barra = 0;
        }

        
        printf("%c ", tab_sol[i]);

        novalinha++;
        barra++;
    }
    printf(" | ");
    printf("\n");
    printf(" | ---------------------- |  \n");
    printf("\n");
  
}

