#include<stdlib.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<time.h>
#include<sys/types.h>
#define TIMEOUT 5 //由于把socket设置为非阻塞，使用select函数，观察其
                //5秒后的是否连接成功，连接不成功则认为其端口没有开放
 
 
 
struct servenet{
    char * s_name;
	char** s_aliases;
	int    s_port;
	char*  s_proto;
};
 
 
//参数是目标机的IP
int main(int argc,char** argv)
{
	struct sockaddr_in server;
	int ret;
	int len;
	int scanport;
    int start_port=0;
    int end_port=1024;
	int sockfd;
 
	fd_set rset;
	fd_set wset; 
        struct servent *sp;
 
	
 
       
    for(scanport=start_port;scanport<end_port;scanport++)
    {
 
 
    	if (-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
		{
		perror("can not create socket\n");
		exit(1);
		}
		
		memset(&server,0,sizeof(struct sockaddr_in));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(argv[1]);
		server.sin_port = htons(scanport);
 		
                int flag = fcntl(sockfd, F_GETFL,0);
		fcntl(sockfd,F_SETFL, flag|O_NONBLOCK);
		struct timeval tm;
		tm.tv_sec = TIMEOUT;
		tm.tv_usec = 0;
        
		//connect为非阻塞，连接不成功立即返回-1
		if (!connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr))){
			 sp=getservbyport(htons(scanport),"tcp");
 
        	printf("tcp port %d open:%s\n ",scanport,sp->s_name);
 
			
                }//假如连接不成功，则运行select,直到超时
		else {
			FD_ZERO(&rset);
			FD_ZERO(&wset);
			FD_SET(sockfd, &rset);
			FD_SET(sockfd, &wset);
			int error; //错误代码
			socklen_t len = sizeof(error); 
			//5秒后查看socket的状态变化 
			if (select(sockfd+1,&rset,&wset,NULL,&tm)>0){
				getsockopt(sockfd, SOL_SOCKET, SO_ERROR,&error, &len );
				if(error == 0)
					printf("Port %d is opened\n", scanport);
			}
		
		}
		close(sockfd);
 
	}
		return 0;
 
}


