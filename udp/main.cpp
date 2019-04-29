//
//  server.c
//  udp
//
//  Created by lovekun on 14-11-2.
//  Copyright (c) 2014年 care.jiangnan. All rights reserved.
//

/* server.c */

#include <stdio.h> /* These are the usual header files */
#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> /* netbd.h is needed for struct hostent =) */
#include <time.h>
#include <iostream>

#define PORT 3000 /* Port that will be opened */
#define MAXDATASIZE 1024*16 /* Max number of bytes of data */


void fps() {
    static auto oldTime = std::chrono::steady_clock::now();
    static int frame=0;
    
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - oldTime);
    frame++;
    if (delta.count() > 1000) {
        printf("%d\n", (frame * 1000 / delta.count()));
        frame = 0;
        oldTime = std::chrono::steady_clock::now();
        
    }
}


int main()
{
    
    
    int sockfd; /* socket descriptors */
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    socklen_t sin_size;
    int num;
    char recvmsg[MAXDATASIZE]; /* buffer for message */
    char sendmsg[MAXDATASIZE];
    char condition[] = "quit";
    /* Creating UDP socket */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        /* handle exception */
        perror("Creating socket failed.");
        exit(1);
    }
    
    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(PORT);
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        /* handle exception */
        perror("Bind error.");
        exit(1);
    }
    
    /* 设置通讯方式对广播，即本程序发送的一个消息，网络上所有主机均可以收到 */
    int yes = 1;
    int fd, numbytes; /* files descriptors */
    
    if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1){ // calls socket()
        printf("socket() error\n");
        exit(1);
    }
    
    setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));
    struct hostent *he; /* structure that will get information about remote host */
    
    if ((he=gethostbyname("127.0.0.1"))==NULL){ // calls gethostbyname()
        printf("gethostbyname() error\n");
        exit(1);
    }
    
    struct sockaddr_in send_server; /* server's address information */
    bzero(&send_server,sizeof(send_server));
    send_server.sin_family = AF_INET;
    send_server.sin_port = htons(10514); /* htons() is needed again */
    send_server.sin_addr = *((struct in_addr *)he->h_addr); /*he->h_addr passes "*he"'s info to "h_addr" */
    
    sin_size=sizeof(struct sockaddr_in);
    while (true) {
        num = recvfrom(sockfd,recvmsg,MAXDATASIZE,0,(struct sockaddr *)&client,&sin_size);
        if (num < 0){
            perror("recvfrom error\n");
            exit(1);
        }
        
        recvmsg[num] = '\0';
        sendto(fd,recvmsg,num,0,(struct sockaddr *)&send_server,sin_size);
        
        fps();
        //        printf("You got a message (%s) from %s\n",recvmsg,inet_ntoa(client.sin_addr) ); /* prints client's IP */
        
    }
    
    close(sockfd); /* close listenfd */
}

