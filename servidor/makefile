# Diretório fonte
DC = source

# Diretório logs
DL = handlers/log

# Diretório Printar Jogo
DPJ = handlers/print_jogo

# Diretório CFG Reader
DCFG = handlers/cfg_reader

# Diretório CFG Reader
DVALIDA = handlers/valida_solucao

# Diretório cJSON
DJSON = libs/JSONLib

# Alvo principal: servidor
servidor: $(DC)/main.o $(DC)/$(DCFG)/configreader.o $(DC)/$(DL)/escrevelogs.o $(DC)/$(DPJ)/printarjogo.o $(DC)/$(DJSON)/cJSON.o $(DC)/$(DVALIDA)/validarsolucao.o $(DC)/socket/socket.o $(DC)/modosjogo/modosjogo.o $(DC)/handlers/statistics/stats.o
    
	gcc -o servidor $(DC)/main.o $(DC)/$(DCFG)/configreader.o $(DC)/$(DL)/escrevelogs.o $(DC)/$(DPJ)/printarjogo.o $(DC)/$(DJSON)/cJSON.o $(DC)/$(DVALIDA)/validarsolucao.o $(DC)/socket/socket.o $(DC)/modosjogo/modosjogo.o $(DC)/handlers/statistics/stats.o -lrt -lm -lpthread

# Compilar o Main
$(DC)/main.o: $(DC)/main.c $(DC)/$(DCFG)/configreader.h $(DC)/socket/socket.h $(DC)/$(DL)/escrevelogs.h $(DC)/$(DPJ)/printarjogo.h $(DC)/global.h $(DC)/modosjogo/modosjogo.h $(DC)/handlers/statistics/stats.h $(DC)/$(DVALIDA)/validarsolucao.h 
  
	gcc -c $(DC)/main.c -o $(DC)/main.o



# Compilar escrevelogs.o
$(DC)/$(DL)/escrevelogs.o: $(DC)/$(DL)/escrevelogs.c $(DC)/$(DL)/escrevelogs.h

	gcc -c $(DC)/$(DL)/escrevelogs.c -o $(DC)/$(DL)/escrevelogs.o

# Compilar printarjogo.o
$(DC)/$(DPJ)/printarjogo.o: $(DC)/$(DPJ)/printarjogo.c $(DC)/$(DPJ)/printarjogo.h

	gcc -c $(DC)/$(DPJ)/printarjogo.c -o $(DC)/$(DPJ)/printarjogo.o

# Compilar configreader.o
$(DC)/$(DCFG)/configreader.o: $(DC)/$(DCFG)/configreader.c $(DC)/$(DCFG)/configreader.h

	gcc -c $(DC)/$(DCFG)/configreader.c -o $(DC)/$(DCFG)/configreader.o

# Compilar Validar Solucao.o
$(DC)/$(DVALIDA)/validarsolucao.o:
	gcc -c $(DC)/$(DVALIDA)/validarsolucao.c -o $(DC)/$(DVALIDA)/validarsolucao.o


# Compilar cJSON.o
$(DC)/$(DJSON)/cJSON.o: $(DC)/$(DJSON)/cJSON.c $(DC)/$(DJSON)/cJSON.h

	gcc -c $(DC)/$(DJSON)/cJSON.c -o $(DC)/$(DJSON)/cJSON.o

#compilar o util.c do socket
$(DC)/socket/socket.o: $(DC)/socket/socket.c $(DC)/socket/socket.h
	gcc -c $(DC)/socket/socket.c -o $(DC)/socket/socket.o

#compilar modos de jogo

$(DC)/modosjogo/modosjogo.o: $(DC)/modosjogo/modosjogo.c $(DC)/modosjogo/modosjogo.h
	gcc -c $(DC)/modosjogo/modosjogo.c -o $(DC)/modosjogo/modosjogo.o

#compilar statistics

$(DC)/handlers/statistics/stats.o: $(DC)/handlers/statistics/stats.c $(DC)/handlers/statistics/stats.h
	gcc -c $(DC)/handlers/statistics/stats.c -o $(DC)/handlers/statistics/stats.o	

# Limpar Lixo "make clean"
clean:
	rm -f $(DC)/*.o servidor
	rm -f $(DC)/$(DL)/*.o
	rm -f $(DC)/$(DCFG)/*.o
	rm -f $(DC)/$(DPJ)/*.o
	rm -f $(DC)/$(DJSON)/*.o
	rm -f $(DC)/$(DVALIDA)/*.o
	rm -f $(DC)/socket/*.o
	rm -f $(DC)/modosjogo/*.o
