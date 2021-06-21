#include "source/socket.h"
#include "source/daemon.h"
#include <pthread.h>

/* Definindo variáveis globais */
static daemonStruct DaemonInfos;
static serverConfig ServerConfig;

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

void signalHandler(int signal);

void configHandler(int* configsArray);

void main() {


    int socketFileDescriptor;
    struct sockaddr_in serverAddr;

    DaemonInfos.daemonName = "TCP-daemon-server";
    DaemonInfos.logFileName = "/var/log/TCP-daemon-server.log";
    DaemonInfos.configFileName = "/etc/TCP-daemon-server/TCP-daemon-server.conf";

    initDaemon(&DaemonInfos, signalHandler, configHandler);

    // Tenta criar o socket
    if(!createSocket(&socketFileDescriptor, &serverAddr, ServerConfig.PORT, NULL)) {
        syslog(LOG_ERR, "Ocorreu um erro na criação do socket");
        syslog(LOG_ERR, "Ocorreu um erro na criação do socket");
        return;
    }
    syslog(LOG_INFO, "Socket criado com sucesso\n");

    // Tenta realziar o bind
    if(!bindSocket(&socketFileDescriptor, &serverAddr)) {
        syslog(LOG_ERR, "Criação do bind falhou");
        return;
    }
    syslog(LOG_INFO, "Bind realizado com sucesso\n");

    //Começa a ouvir a porta predefinida
    if (!startListen(&socketFileDescriptor,  ServerConfig.MAX_CONNECTIONS)) {
        syslog(LOG_INFO, "Falha no inicio da escuta\n");
        return;
    }
    syslog(LOG_INFO, "Escutando conexões\n");

    syslog(LOG_INFO, "Iniciando rotina principal do servidor\n");
    threadCreationRoutine(socketFileDescriptor, serverAddr);

    close(socketFileDescriptor);
    finishDaemonRoutine(&DaemonInfos);
}

void signalHandler(int signal) {

    syslog(LOG_NOTICE, "Signal [ %d ] recebido.", signal);

    switch (signal) {
        case SIGINT:
            syslog(LOG_INFO, "Parando o servidor");
            ServerConfig.IS_RUNNING = 0;
            break;
        case SIGHUP:
            syslog(LOG_INFO, "Recarregando servidor");
            //readConfigFile(configFileName);
            break;
        default :
            break;
    }
}

void configHandler(int* configsArray) {
    ServerConfig.MAX_MESSAGE_SIZE = configsArray[0];
    ServerConfig.PORT = configsArray[1];
    ServerConfig.MAX_CONNECTIONS = configsArray[2];
    ServerConfig.IS_RUNNING = 1;
}

void threadCreationRoutine(int socketFileDescriptor, struct sockaddr_in serverAddr) {
    //loop de funcionamento do servidor
    while(ServerConfig.IS_RUNNING) {

        int* connectionFileDescriptor = (int*)malloc(sizeof(int));

        //Espera até uma conexão aparecer e aceita
        if (ServerConfig.IS_RUNNING >0 && !acceptConection(&socketFileDescriptor, &serverAddr, connectionFileDescriptor)) {
            syslog(LOG_ERR, "Falha na conexão\n");
            free(connectionFileDescriptor);
            continue;
        }
        syslog(LOG_INFO, "Conexão estabelecida com sucesso\n");

        int rcr;
        pthread_t serverThread;

        //cria uma nova thread que roda a rotina de servidor
        if ( (rcr = pthread_create(&serverThread, NULL, serverRoutine, (void *)connectionFileDescriptor)) ){
            syslog(LOG_INFO, "Criação da thread falhou: %d\n", rcr);
        }
    
    }
}

void *serverRoutine(void* connectionFileDescriptorPointer) {

    int* connectionFileDescriptor = (int*)connectionFileDescriptorPointer;
    char message[ServerConfig.MAX_MESSAGE_SIZE];

    send(*connectionFileDescriptor, "Conexão recebida com sucesso", ServerConfig.MAX_MESSAGE_SIZE, 0);
    syslog(LOG_INFO, "Iniciando conexão com o cliente %d\n", *connectionFileDescriptor);

    int receivedBytes;

    while(1) {
        bzero(message, ServerConfig.MAX_MESSAGE_SIZE);
        
        receivedBytes = recv(*connectionFileDescriptor, message, sizeof(message), 0);
        if(receivedBytes <= 0) break;

        syslog(LOG_INFO, "Recebido do cliente %d: %s\n", *connectionFileDescriptor, message);
        sleep(1);
        
        send(*connectionFileDescriptor, message, sizeof(message), 0);
        syslog(LOG_INFO, "Enviando para o cliente  %d: %s\n", *connectionFileDescriptor, message);

    };

    syslog(LOG_INFO, "Fechando conexão com o cliente: %d\n", *connectionFileDescriptor);
    close(*connectionFileDescriptor);
    free(connectionFileDescriptorPointer);
}

