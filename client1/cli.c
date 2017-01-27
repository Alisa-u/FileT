#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);

	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;//地址族
	saddr.sin_port=htons(6500);//用和服务器一样的端口号.6500指的是要链接的服务器是6500，客户端的端口号是随机分配
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");	
		
	int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));// int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);三次握手
	assert(res!=-1);//connect成功代表三次握手成功
	while(1)
	{
		printf("input:\n");
		char buff[128]={0};
		fgets(buff,128,stdin);//从键盘中获取
		if(strncmp(buff,"end",3)==0)
		{
			break;//退出
		}
		send(sockfd,buff,strlen(buff),0);//写到sockfd套接字中
		memset(buff,0,128);
		recv(sockfd,buff,127,0);//读取服务器发来的信息
		printf("buff=%s\n",buff);//打印
			
	}
close(sockfd);//关闭
}
