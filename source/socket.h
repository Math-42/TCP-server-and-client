#ifndef _TCP_SOCKET_
#define _TCP_SOCKET_
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct clientConnection{
	int connectionFileDescriptor;
	int clientId;
	pthread_mutex_t *mutex;
}clientConnection;

typedef struct serverConfig{
	int MAX_MESSAGE_SIZE;
	int PORT;
	int MAX_CONNECTIONS;
	int IS_RUNNING ;
} serverConfig;


/**
 * Cria um socket e seta suas configurações de endereço
 * @param socketFileDescriptor file descriptor
 * @param socketAddr struct do endereço do socket
 * @param PORT porta que será utilizada para estabelecer a conexão
 * @param address endereço da conexão, caso seja NULL usará o IP local
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int createSocket(int* socketFileDescriptor,struct sockaddr_in *socketAddr, unsigned short PORT, char* address);

/**
 * Associa o socket a uma porta no computador
 * @param socketFileDescriptor file descriptor
 * @param socketAddr struct do endereço do socket
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int bindSocket(int* socketFileDescriptor, struct sockaddr_in *socketAddr);

/**
 * Associa o socket a uma porta no computador
 * @param socketFileDescriptor file descriptor
 * @param socketAddr struct do endereço do socket
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int bindSocket(int* socketFileDescriptor, struct sockaddr_in *socketAddr);

/**
 * Começa a ouvir conexões na porta predefinida
 * @param socketFileDescriptor file descriptor
 * @param numberOfConnections numero de conexões que serão irão esperar até serem aceitas
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int startListen(int* socketFileDescriptor, int numberOfConnections);

/**
 * Começa a ouvir conexões na porta predefinida
 * @param socketFileDescriptor file descriptor
 * @param socketAddr struct contendo os dados do servidor que se deseja conectar
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int connectToServer(int* socketFileDescriptor,struct sockaddr_in *serverAddr);

/**
 * Começa a ouvir conexões na porta predefinida
 * @param socketFileDescriptor file descriptor
 * @param socketAddr numero de conexões que irão esperar até serem aceitas
 * @param connectionFileDescriptor file desciptor da conexão estabelecida 
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int acceptConection(int* socketFileDescriptor, struct sockaddr_in *socketAddr, int * connectionFileDescriptor);


#endif