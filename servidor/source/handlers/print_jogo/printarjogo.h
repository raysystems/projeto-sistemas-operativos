//Evitar includes ciclicos
#ifndef PRINTARJOGO_H
#define PRINTARJOGO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../libs/JSONLib/cJSON.h"
#include "../log/escrevelogs.h"


char* abrirficheiroJSON (const char* caminhoFichiero, char* logs_nomeficheiro);

char** obterarrayjogos(const char* json, int idjogoProcurado, char* logs_nomeficheiro);

void enviarParaEcra(char* tab_sol);

#endif