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

int bindSocket(int* socketFileDescriptor, struct sockaddr_in *socketAddr){
	int yes=1;
	if(bind(*socketFileDescriptor, (struct sockaddr *)socketAddr, sizeof(*socketAddr)) == 0) return 1;//tenta criar o bind
	return setsockopt(*socketFileDescriptor,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1 ? 0:1;//caso falhe tenta reaproveitar o endereço
}

int startListen(int* socketFileDescriptor, int numberOfConnections){
	return listen(*socketFileDescriptor, numberOfConnections) == -1 ? 0:1;
}

int connectToServer(int* socketFileDescriptor,struct sockaddr_in *serverAddr){
	return connect(*socketFileDescriptor, (struct sockaddr*)serverAddr, sizeof(*serverAddr)) == -1 ? 0:1;
}

int acceptConection(int* socketFileDescriptor, struct sockaddr_in *socketAddr, int * connectionFileDescriptor){
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof their_addr;
	*connectionFileDescriptor = accept(*socketFileDescriptor, (struct sockaddr*)&their_addr, &addr_size);
	return *connectionFileDescriptor == -1 ? 0:1;
}
