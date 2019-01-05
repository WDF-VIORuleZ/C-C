output: main.o Server.o Client.o tokens.o
	g++ main.o Server.o Client.o tokens.o -o ./o/SERVER_DBG

main.o:
	g++ -c ./src/main.cc

Server.o: ./src/Server.cc ./header/Server.hh ../general_includes/logging.h
	g++ -c ./src/Server.cc 

Client.o: ../client/Client.cc ../client/Client.hh
	g++ -c ../Client/Client.cc

tokens.o: ../general_includes/tokens.cc ../general_includes/tokens.hh
	g++ -c ../general_includes/tokens.cc

clean:
	rm 