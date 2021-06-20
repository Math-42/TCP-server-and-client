#include "source/socket.h"

void main() {
    int socketFileDescriptor;
    struct sockaddr_in serverAddr;

    // Tenta criar o socket
    if (!createSocket(&socketFileDescriptor, &serverAddr, PORT, "127.0.0.1")) {
        perror("Ocorreu um erro na criação do socket");
        return;
    }
    printf("Socket criado\n");

    // Tenta criar o socket
    if (!connectToServer(&socketFileDescriptor, &serverAddr)) {
        perror("Ocorreu um erro na conexão com o servidor");
        return;
    }
    printf("Conexão estabelecida\n");

    clientRoutine(socketFileDescriptor);

    close(socketFileDescriptor);
}
