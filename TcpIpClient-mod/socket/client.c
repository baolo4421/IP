#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 30
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSz;
	
	SOCKADDR_IN clntAdr, servAdr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		ErrorHandling("WSAStartup() error!"); 

	sock=socket(PF_INET, SOCK_DGRAM, 0);   
	if(sock==INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=inet_addr(argv[1]);
	servAdr.sin_port=htons(atoi(argv[2]));
	
	connect(sock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while(1)
	{
		while(1) 
		{
			fputs("Insert message(q to quit): ", stdout);
			fgets(message, sizeof(message), stdin);    
			break;

		}
		send(sock, message, strlen(message), 0);

		clntAdrSz=sizeof(clntAdr);
		strLen=recvfrom(sock, message, BUF_SIZE, 0, 
								(SOCKADDR*)&clntAdr, &clntAdrSz);
		message[strLen]=0;
		
		sendto(sock, message, strLen, 0, 
								(SOCKADDR*)&clntAdr, sizeof(clntAdr));

		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;

		printf("Message from server: %s", message);
		//strLen=recv(sock, receive_message, sizeof(receive_message)-1, 0);
		//
  //      if (strLen == SOCKET_ERROR){
  //          printf("recv Error\n");
  //          break;
  //      }
  //      receive_message[strLen]=0;

		//if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
		//	break;
		//printf("Message from server: %s", receive_message);
	}	
	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
