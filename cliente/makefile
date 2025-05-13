# Diretório fonte
DC = source

# Diretorio logs
DL = handlers/log

DPRINT = handlers/print_jogo

# Diretorio CFG Reader

DCFG = handlers/cfg_reader

cliente: $(DC)/main.o $(DC)/$(DCFG)/configreader.o $(DC)/$(DL)/escrevelogs.o $(DC)/socket/socket.o $(DC)/jogo/jogo.o $(DC)/$(DPRINT)/printjogo.o
    
	gcc -o cliente $(DC)/main.o $(DC)/$(DCFG)/configreader.o $(DC)/$(DPRINT)/printjogo.o $(DC)/$(DL)/escrevelogs.o $(DC)/socket/socket.o $(DC)/jogo/jogo.o -lpthread

# Compilar o Main
$(DC)/main.o: $(DC)/main.c $(DC)/$(DCFG)/configreader.h $(DC)/socket/socket.h $(DC)/jogo/jogo.h  $(DC)/$(DL)/escrevelogs.h
  
	gcc -c $(DC)/main.c -o $(DC)/main.o

#compilar escrevelogs.o
$(DC)/$(DL)/escrevelogs.o: $(DC)/$(DL)/escrevelogs.c $(DC)/$(DL)/escrevelogs.h

	gcc -c $(DC)/$(DL)/escrevelogs.c -o $(DC)/$(DL)/escrevelogs.o

$(DC)/$(DCFG)/configreader.o: $(DC)/$(DCFG)/configreader.c $(DC)/$(DCFG)/configreader.h

	gcc -c $(DC)/$(DCFG)/configreader.c -o $(DC)/$(DCFG)/configreader.o

#compilar print_jogo

$(DC)/$(DPRINT)/printjogo.o: $(DC)/$(DPRINT)/printjogo.c $(DC)/$(DPRINT)/printjogo.h
	gcc -c $(DC)/$(DPRINT)/printjogo.c -o $(DC)/$(DPRINT)/printjogo.o
# Limpar Lixo "make clean"

#compilar socket
$(DC)/socket/socket.o: $(DC)/socket/socket.c $(DC)/socket/socket.h
	gcc -c $(DC)/socket/socket.c -o $(DC)/socket/socket.o

#compilar jogo
$(DC)/jogo/jogo.o: $(DC)/jogo/jogo.c $(DC)/jogo/jogo.h
	gcc -c $(DC)/jogo/jogo.c -o $(DC)/jogo/jogo.o

clean:
  
	rm -f $(DC)/*.o cliente
	rm -f $(DC)/$(DL)/*.o
	rm -f $(DC)/$(DCFG)/*.o
	rm -f $(DC)/socket/*.o
	rm -f $(DC)/$(DPRINT)/*.o
	rm -f $(DC)/jogo/*.o
