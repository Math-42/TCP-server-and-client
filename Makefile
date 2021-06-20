all: clear server client

server:
	gcc -pthread source/socket.c server.c -o server
client:
	gcc source/socket.c client.c -o client
clear:
	rm -f client && rm -f server
run:
	./server