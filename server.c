#include "source/socket.h"
#include <pthread.h>

#define MAX_MESSAGE_SIZE 80
#define PORT 8081
#define MAX_CONNECTIONS 10

/**
 * Ouve e responde as mensagens recebidas pelos clientes
 * @param connectionFileDescriptorPointer ponteiro para o file descriptor gerado para a conexão em especifico
 */
void *serverRoutine(void* connectionFileDescriptorPointer);

/**
 * Rotina que cria uma thread para lidar com cada uma das conexões que o servidor recebe
 * @param socketFileDescriptor file descriptor do próprio servidor
 * @param serverAddr struct contendo os dados do servidor
 */
void threadCreationRoutine(int socketFileDescriptor, struct sockaddr_in serverAddr);

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
    threadCreationRoutine(socketFileDescriptor, serverAddr);

    close(socketFileDescriptor);
}

void threadCreationRoutine(int socketFileDescriptor, struct sockaddr_in serverAddr) {
    //loop de funcionamento do servidor
    while(1) {

        int* connectionFileDescriptor = (int*)malloc(sizeof(int));

        //Espera até uma conexão aparecer e aceita
        if (!acceptConection(&socketFileDescriptor, &serverAddr, connectionFileDescriptor)) {
            perror("Falha na conexão\n");
            free(connectionFileDescriptor);
            continue;
        }
        printf("Conexão estabelecida com sucesso\n");

        int rcr;
        pthread_t serverThread;

        //cria uma nova thread que roda a rotina de servidor
        if ( (rcr = pthread_create(&serverThread, NULL, serverRoutine, (void *)connectionFileDescriptor)) ){
            printf("Criação da thread falhou: %d\n", rcr);
        }
    
    }
}

void *serverRoutine(void* connectionFileDescriptorPointer) {

    int* connectionFileDescriptor = (int*)connectionFileDescriptorPointer;

    char message[MAX_MESSAGE_SIZE];
    send(*connectionFileDescriptor, "Conexão recebida com sucesso", MAX_MESSAGE_SIZE, 0);
    printf("Abrindo conexão com o cliente %d\n", *connectionFileDescriptor);

    while (1) {
        bzero(message, MAX_MESSAGE_SIZE);
        
        if(recv(*connectionFileDescriptor, message, sizeof(message), 0) == 0){
            break;
        };

        printf("Recebido do cliente %d: %s\n", *connectionFileDescriptor, message);
        sleep(1);
        
        send(*connectionFileDescriptor, message, sizeof(message), 0);
        printf("Enviando para o cliente  %d: %s\n", *connectionFileDescriptor, message);
    }

    printf("Fechando conexão com o cliente: %d\n", *connectionFileDescriptor);
    close(*connectionFileDescriptor);
    free(connectionFileDescriptorPointer);
}

