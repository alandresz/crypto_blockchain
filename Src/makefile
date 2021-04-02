
OBJECTS = main.o sha256.o cmdline.o  Queue.o TransactionInput.o TransactionOutput.o Transaction.o Block.o Mempool.o BlockChainUser.o BlockChainBuilder.o BlockChainFileManager.o BlockChainManager.o BlockChainBookkeeper.o BlockChainHistoryBook.o # Los archivos compilados individuales
PROGR = miner.exe # Nombre del archivo ejecutable
CPPFLAGS = -g -Wall -pedantic -Werror # -g opcion de g++ para debugear

# Compiladores #
CC = g++ -std=c++17 # Para linux
CCW = i686-w64-mingw32-g++ --static # Para windows (requiere mingw32)

$(PROGR) : $(OBJECTS)
	$(CC) $(CPPFLAGS) -o $(PROGR) $(OBJECTS)
main.o : main.cpp cmdline.h BlockChainManager.h
	$(CC) $(CPPFLAGS) -c main.cpp
cmdline.o : cmdline.cc cmdline.h
	$(CC) $(CPPFLAGS) -c cmdline.cc
sha246.o : sha246.cpp sha246.h
	$(CC) $(CPPFLAGS) -c sha246.cpp
queue.o : Queue.cpp Queue.h
	$(CC) $(CPPFLAGS) -c Queue.cpp
Mempool.o : Mempool.cpp Mempool.h lista.h
	$(CC) $(CPPFLAGS) -c Mempool.cpp
BlockChainUser.o : BlockChainUser.cpp BlockChainUser.h
	$(CC) $(CPPFLAGS) -c BlockChainUser.cpp
BlockChainManager.o : BlockChainManager.cpp BlockChainManager.h BlockChainFileManager.h BlockChainBuilder.h BlockChainBookkeeper.h BlockChainStatus.h 
	$(CC) $(CPPFLAGS) -c BlockChainManager.cpp
BlockChainFileManager.o : BlockChainFileManager.cpp BlockChainFileManager.h BlockChainBuilder.h BlockChainDataTypes.h BlockChainStatus.h Queue.h
	$(CC) $(CPPFLAGS) -c BlockChainFileManager.cpp
BlockChainBuilder.o : BlockChainBuilder.cpp BlockChainBuilder.h lista.h sha256.h Block.h TiposHash.h BlockChainDataTypes.h BlockChainStatus.h
	$(CC) $(CPPFLAGS) -c BlockChainBuilder.cpp
BlockChainBookkeeper.o : BlockChainBookkeeper.cpp BlockChainBookkeeper.h BlockChainDataTypes.h BlockChainStatus.h
	$(CC) $(CPPFLAGS) -c BlockChainBookkeeper.cpp
BlockChainHistoryBook.o : BlockChainHistoryBook.cpp BlockChainHistoryBook.h Block.h lista.h
	$(CC) $(CPPFLAGS) -c BlockChainHistoryBook.cpp
Block.o : Block.cpp Block.h Transaction.h lista.h TiposHash.h BlockChainDataTypes.h
	$(CC) $(CPPFLAGS) -c Block.cpp
Transaction.o : Transaction.cpp Transaction.h lista.h TransactionOutput.h TransactionInput.h  BlockChainDataTypes.h
	$(CC) $(CPPFLAGS) -c Transaction.cpp
TransactionOutput.o : TransactionOutput.cpp TransactionOutput.h
	$(CC) $(CPPFLAGS) -c TransactionOutput.cpp
TransactionInput.o : TransactionInput.cpp TransactionInput.h
	$(CC) $(CPPFLAGS) -c TransactionInput.cpp	


clean:
	rm -f core $(PROGR) $(OBJECTS)
all: $(PROGR)
	$(CC) $(CPPFLAGS) -o $(PROGR) $(OBJECTS)
memcheck : $(PROGR)
	valgrind  --leak-check=yes ./$(PROGR) -i input.txt -o output.txt
memcheck_s : $(PROGR)
	valgrind -s --leak-check=yes ./$(PROGR) -i input.txt -o output.txt
std_out : $(PROGR)
	./$(PROGR) -i input.txt -o -
std_in : $(PROGR)
	./$(PROGR) -i - -o output.txt
std : $(PROGR)
	./$(PROGR) -i - -o -

