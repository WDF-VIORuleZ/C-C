output: main.o Client.o
	g++ main.o Client.o -o ./o/CLIENT_DBG

main.o:
	g++ -c ./src/main.cc

Client.o: ./src/Client.cc ./header/Client.hh
	g++ -c ./src/Client.cc

clean:
	rm *.o 