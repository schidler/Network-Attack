all: real
real: real.cpp show.cpp
	g++  -o real real.cpp show.cpp client.o  -L. -L/usr/lib -lm -lcurl -lxmlrpc_client -lxmlrpc -lxmlrpc_util -lxmlrpc_xmlparse -lxmlrpc_xmltok -I.
clean:
	rm real
