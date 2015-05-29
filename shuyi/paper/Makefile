all: real
real: real.o show.o client.o
	g++ -o real real.o show.o client.o  -L. -L/usr/lib -lm -lcurl -lxmlrpc_client -lxmlrpc -lxmlrpc_util -lxmlrpc_xmlparse -lxmlrpc_xmltok -I.

client.o: client.cpp
	g++ -c -o client.o client.cpp -I.

real.o: real.cpp
	g++  -c -o real.o real.cpp 
show.o: show.cpp
	g++ -c -o show.o show.cpp -I.
clean:
	rm real
run: real
	./real < data_3
