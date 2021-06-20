#include "source/socket.h"
#define MAX_MESSAGE_SIZE 80
#define PORT 8081
#define MAX_CONNECTIONS 10
void main() {

    int socketFileDescriptor;
    struct sockaddr_in serverAddr;

    // Tenta criar o socket
    if(!createSocket(&socketFileDescriptor, &serverAddr, 8080, NULL)) {
        perror("Ocorreu um erro na criação do socket");
        return;
    }
    printf("Socket criado com sucesso\n");

