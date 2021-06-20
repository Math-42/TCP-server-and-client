#ifndef _TCP_SOCKET_
#define _TCP_SOCKET_
#include "socket.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * Cria um socket e seta suas configurações de endereço
 * @param socketFileDescriptor file descriptor
 * @param socketAddr struct do endereço do socket
 * @param PORT porta que será utilizada para estabelecer a conexão
 * @param address endereço da conexão, caso seja NULL usará o IP local
 * @return retorna 0 caso ocorra algum erro e 1 caso de certo
 */
int createSocket(int* socketFileDescriptor,struct sockaddr_in *socketAddr, unsigned short PORT, char* address);

#endif