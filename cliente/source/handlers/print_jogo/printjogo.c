#include "printjogo.h"
#include <stdio.h>
#include <string.h>


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