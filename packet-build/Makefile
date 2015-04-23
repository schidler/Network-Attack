all: build_tcp
CC = gcc 
lib = -lnet
obj = build_tcp
build_tcp: build_tcp.c
	$(CC) -o build_tcp build_tcp.c $(lib)
clean: 
	rm $(obj)

run: build_tcp
	./build_tcp
