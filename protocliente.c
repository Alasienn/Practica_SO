#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

#define MAXLEN 32
#define PORT 3535



int menu() {
    printf("\n.:*************:.");
    printf("\n   1. Ingresar registro.");
    printf("\n   2. Ver registro.");
    printf("\n   3. Borrar Registro.");
    printf("\n   4. Buscar Registro.");
    printf("\n   5. Salir.");
    printf("\n   ________________________");
    printf("\n\n   Introduzca opción (1-5): ");

}

int main(){
	int clientfd,r;
	char msg[32];
	struct sockaddr_in client;
	socklen_t tama=sizeof(struct sockaddr);
	clientfd=socket(AF_INET,SOCK_STREAM,0);
	if(clientfd==-1)
	{
		perror("error en socket\n");
	}
	else
	{
		client.sin_family=AF_INET;
		client.sin_port=htons(PORT);
		client.sin_addr.s_addr=inet_addr("127.0.0.1");
		bzero(client.sin_zero,8);
		r=connect(clientfd,(struct sockaddr*)&client,tama);
		if(r==-1)
		{
			perror("error en connect\n");
		}
		else
		{
			do
			{
        		menu();
				fgets(msg, 32, stdin);
				send(clientfd, msg, 32, 0); //envio
				bzero(msg, 32);
				r=recv(clientfd,msg,MAXLEN,0);
				printf("%s %d\n", msg,r);
			}while( r >0);
			if(r==-1)
			{
				perror("error en recv\n");
			}
		}
	}
	close(clientfd);
}
