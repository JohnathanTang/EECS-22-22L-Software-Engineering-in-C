 /*sys/types.h：数据类型定义
 
sys/socket.h：提供socket函数及数据结构
 
netinet/in.h：定义数据结构sockaddr_in
 
arpa/inet.h：提供IP地址转换函数
 
netdb.h：提供设置及获取域名的函数
 
sys/ioctl.h：提供对I/O控制的函数
  
sys/poll.h：提供socket等待测试机制的函数

unistd.h：提供通用的文件、目录、程序及进程操作的函数
 
errno.h：提供错误号errno的定义，用于错误处理
 
fcntl.h：提供对文件控制的函数
 
time.h：提供有关时间的函数
 
crypt.h：提供使用DES加密算法的加密函数
 
pwd.h：提供对/etc/passwd文件访问的函数
 
shadow.h：提供对/etc/shadow文件访问的函数
 
pthread.h：提供多线程操作的函数
 
signal.h：提供对信号操作的函数
 
sys/wait.h、sys/ipc.h、sys/shm.h：提供进程等待、进程间通讯（IPC）及共享内存的函数
*/
#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>



int main()
{
	char Message[1024];
	struct sockaddr_in ServerAddr;
	bzero(&ServerAddr,sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(1314);
	ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	fd_set rset, allset;
	int maxfd;
	
	
	int ClientSocket=socket(AF_INET,SOCK_STREAM,0);
	if(ClientSocket<0)
	{
		printf("ERROR:ClientSocket\n");
		return -1;
		
	}
	printf("ClientSocket Created");
	
	int Connection=connect(ClientSocket,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));
	if(Connection<0)
	{
		printf("ERROR:Connection\n");
		close(ClientSocket);
		return -1;
	}
	FD_ZERO(&allset);
	FD_SET(ClientSocket, &allset);
	FD_SET(STDIN_FILENO, &allset);
	maxfd=(ClientSocket>STDIN_FILENO)?ClientSocket:STDIN_FILENO;
	
	
	
	while(1)
	{
		rset=allset;

		if(select(maxfd+1,&rset,NULL,NULL,NULL))
		{
			if(FD_ISSET(ClientSocket,&rset))
			{
				memset(Message,0,1024);
				Connection=recv(ClientSocket,Message,sizeof(Message),0);
				if(Connection<0)
				{
					printf("ERROR:Connection while receiving Message\n");
					close(ClientSocket);
					return -1;
				}
				printf("%s\n",Message);
			}
			else if(FD_ISSET(STDIN_FILENO,&rset))
			{
				memset(Message,0,1024);
				gets(Message);
				Connection=send(ClientSocket,Message,sizeof(Message),0);
				if(strcmp(Message,"quit")==0)
				{
				 break;
				}
				if(Connection<0)
				{
					printf("ERROR:Connection while sending Message\n");
					close(ClientSocket);
					return -1;
				}
			}
		}
		
	}
	close(ClientSocket);
	return 0;
}





















