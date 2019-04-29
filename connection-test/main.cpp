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
#include<iostream>
#include<sstream>
#include<sys/errno.h>

#define TIMEOUT 5 //由于把socket设置为非阻塞，使用select函数，观察其
//5秒后的是否连接成功，连接不成功则认为其端口没有开放



extern char** environ;

//参数是目标机的IP
int main(int argc,char** argv)
{
    int nIndex = 0;
    
    for(nIndex = 0; environ[nIndex] != NULL; nIndex++)
    {
        printf("%s\n",environ[nIndex]);
    }
    
    using namespace std;
    struct sockaddr_in server;
    int scanport;
    int sockfd;
    
    fd_set rset;
    fd_set wset;
    struct servent *sp;
    
    if(argc<3)
    {
        cout<<"argument error\n";
        return 1;
    }
    stringstream ss;
    ss<<argv[2];
    ss>>scanport;
    
    
    
    if (-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
    {
        perror("can not create socket\n");
        return 1;
    }
    
    memset(&server,0,sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons(scanport);
    
    //            int flag = fcntl(sockfd, F_GETFL,0);
    //            fcntl(sockfd,F_SETFL, flag|O_NONBLOCK);
    struct timeval tm;
    tm.tv_sec = TIMEOUT;
    tm.tv_usec = 0;
    
    //connect为非阻塞，连接不成功立即返回-1
    int ret=connect(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr));
    if (ret==0){
        sp=getservbyport(htons(scanport),"tcp");
        
        printf("tcp port %d open:%s\n ",scanport,sp->s_name);
        ret= 0;
        
    }//假如连接不成功，则运行select,直到超时
    else
    {
        printf("connect: %d, %s\n", errno, strerror(errno));
        ret= 1;
        
    }
    close(sockfd);
    
    return ret;
    
    
}


