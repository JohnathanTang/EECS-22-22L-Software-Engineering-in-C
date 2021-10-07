#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#define UserNameLen 10
#define MaxClient 10

typedef struct sock Sock;
struct sock
{
	int s1;
	int s2;
};
void *Chat( void *ConnectID);


int main()
{
	pthread_t ChatThreads[2];
	struct sockaddr_in ServerAddr,ClientAddr;
	bzero(&ClientAddr,sizeof(ClientAddr));
	bzero(&ServerAddr,sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(1314);
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	int ConnectedSocket1,ConnectedSocket2;
	int ServerSocket=socket(AF_INET,SOCK_STREAM,0);
	
	
	if(ServerSocket<0)
	{
		printf("ERROR:ServerSocket\n");
		return -1;
	}
	int Connection=bind(ServerSocket,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));
	if(Connection<0)
	{
		printf("ERROR:Connection\n");
		close(ServerSocket);
		return -1;
	}
	if(listen(ServerSocket,10)<0)
    {
      printf("ERROR: listen\n");
      close(ServerSocket);
      return -1;
    }
	ConnectedSocket1=accept(ServerSocket,NULL,NULL);
	ConnectedSocket2=accept(ServerSocket,NULL,NULL);
	Sock arg1;
	Sock arg2;
	arg1.s1=ConnectedSocket1;
	arg1.s2=ConnectedSocket2;
	arg2.s1=ConnectedSocket2;
	arg2.s2=ConnectedSocket1;
	pthread_create(&ChatThreads[0],NULL,Chat,(void *)&arg1);
	pthread_create(&ChatThreads[1],NULL,Chat,(void *)&arg2);
 	
	pthread_join(ChatThreads[0],NULL);
	pthread_join(ChatThreads[1],NULL);
	
	
	
}


void *Chat( void *ConnectID) 
{
	Sock IDarg=*(Sock *)ConnectID;
	char Message[1024];
	int Connection;
	while(1)
	{
		memset(Message,0,1024);
		Connection=recv(IDarg.s1,Message,sizeof(Message),0);
		if(Connection<0)
		{
			printf("ERROR:Connection while receiving Message\n");
			close(IDarg.s1);
			return;
		}
		Connection=send(IDarg.s2,Message,sizeof(Message),0);
		if(Connection<0)
		{
			printf("ERROR:Connection while sending Message\n");
			close(IDarg.s1);
			return;
		}
	}
	
	
	
}


















