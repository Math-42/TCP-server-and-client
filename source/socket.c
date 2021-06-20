#include "socket.h"

int createSocket(int* socketFileDescriptor,struct sockaddr_in *socketAddr, unsigned short PORT, char* address) {

	*socketFileDescriptor = socket(PF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);//Dominio do socket, tipo do socket, protocolo

	if(*socketFileDescriptor == -1) return 0;

	bzero(socketAddr, sizeof(*socketAddr));//seta toda memória como 0

    socketAddr->sin_family = AF_INET;  //Setando como IPv4
    socketAddr->sin_addr.s_addr = (address == NULL)? htonl(INADDR_ANY): inet_addr(address); //Endereço especial que permite rodar sem saber o ip da maquina
    socketAddr->sin_port = htons(PORT);//Seta a porta utilizando Network Byte Order AKA Big Endian

	return 1;
}
