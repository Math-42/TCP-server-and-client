#include "source/socket.h"
#define MAX_MESSAGE_SIZE 80
#define PORT 8081
#define MAX_CONNECTIONS 10
void main() {

    int socketFileDescriptor;
    struct sockaddr_in serverAddr;

    // Tenta criar o socket
    if(!createSocket(&socketFileDescriptor, &serverAddr, PORT, NULL)) {
        perror("Ocorreu um erro na criação do socket");
        return;
    }
    printf("Socket criado com sucesso\n");

    // Tenta realziar o bind
    if(!bindSocket(&socketFileDescriptor, &serverAddr)) {
        perror("Criação do bind falhou");
        return;
    }
    printf("Bind realizado com sucesso\n");

    //Começa a ouvir a porta predefinida
    if (!startListen(&socketFileDescriptor, MAX_CONNECTIONS)) {
        printf("Falha no inicio da escuta\n");
        return;
    }
    printf("Escutando conexões\n");

    printf("Iniciando rotina principal do servidor\n");
}
