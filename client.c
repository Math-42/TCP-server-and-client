#include "source/socket.h"

#define MAX_MESSAGE_SIZE 80
#define PORT 3050

/**
 * Envia e escuta mensagens do servidor
 * @param socketFileDescriptor file descriptor gerado para a conexão com o servidor
 */
void clientRoutine(int socketFileDescriptor);

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

void clientRoutine(int socketFileDescriptor) {
    char message[MAX_MESSAGE_SIZE];
    bzero(message, MAX_MESSAGE_SIZE);

    read(socketFileDescriptor, message, sizeof(message));
    printf("Servidor : %s\n", message);

    while (1) {
        bzero(message, MAX_MESSAGE_SIZE);

        printf("Mensagem: ");
        scanf("%s", message);

        if(send(socketFileDescriptor, message, sizeof(message), 0) <= 0) break;
        bzero(message, MAX_MESSAGE_SIZE);

        if(recv(socketFileDescriptor, message, sizeof(message), 0) <= 0) break;
        printf("Recebido do servidor: %s\n", message);
    }

    printf("Conexão perdida...\n");
}