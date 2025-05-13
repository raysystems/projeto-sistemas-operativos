#include "validarsolucao.h"

int validarjogo(char* jogo) {
    int faltacasas = 0;

    for (int i = 0; i < 81; i++) {
        if (jogo[i] == '0') {
         
            faltacasas = 1;
            break;
            
        }
    }

    return faltacasas;
}