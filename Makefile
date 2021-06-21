all: clear server client

server:
	gcc -pthread source/daemon.c source/socket.c server.c -o TCP-daemon-server
client:
	gcc source/socket.c client.c -o TCP-client
clear:
	rm -f client && rm -f TCP-daemon-server
run:
	./server