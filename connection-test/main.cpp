//
// Created by darren on 18-8-22.
//

#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>


int getLocalPort() {
    unsigned short port = 0;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return port;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = 0;        // 若port指定为0,则调用bind时，系统会为其指定一个可用的端口号
    int ret = bind(sock, (struct sockaddr *) &addr, sizeof addr);    // 3. 绑定
    do {
        if (0 != ret) {// 4. 利用getsockname获取
            //LOGE("upn bind, %d, %s", errno, strerror(errno))
            break;
        }
        struct sockaddr_in sockaddr;
        int len = sizeof(sockaddr);
        ret = getsockname(sock, (struct sockaddr *) &sockaddr, (socklen_t *) &len); // this function now not be hooked
        
        if (0 != ret) {
            // LOGE("upn, getsockname:%d, %s", errno, strerror(errno))
            break;
        }
        port = ntohs(sockaddr.sin_port); // 获取端口号
    } while (false);
    //LOGD("UpnConfig::port::%d", port)
    close(sock);
    
    return port;
}

int getConnectState(int port, const char *ip, int tv_sec, int tv_usec) {
    
    int fd = -1;
    int ret = 0;
    do {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            printf("Test::connect:socket: %d, %s\n", errno, strerror(errno));
            break;
        }
        struct sockaddr_in addr;
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);
        
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags < 0) {
            printf("Test::connect:get flags: %d, %s\n", errno, strerror(errno));
            break;
        }
        flags |= O_NONBLOCK;
        if (fcntl(fd, F_SETFL, (void *) flags) < 0) {
            printf("Test::connect:set flags: %d, %s\n", errno, strerror(errno));
            break;
        }
        if (0 == connect(fd, (struct sockaddr *) &addr, sizeof(addr))) {
            break;
        }
        printf("Test::connect:failed: %d, %s\n", errno, strerror(errno));
#ifdef __ENABLE_LOG_D__
        if (EINPROGRESS != errno) {
            LOGE("Test::connect:failed: %d, %s", errno, strerror(errno));
            break;
        }
#endif
        fd_set fdr, fdw;
        FD_ZERO(&fdr);
        FD_ZERO(&fdw);
        FD_SET(fd, &fdr);
        FD_SET(fd, &fdw);
        
        struct timeval timeout;
        timeout.tv_sec = tv_sec;// seconds
        timeout.tv_usec = tv_usec;
        int rc = select(fd + 1, &fdr, &fdw, NULL, &timeout);
        if (0 < rc && (FD_ISSET(fd, &fdw) || FD_ISSET(fd, &fdr))) {
            break;
        }
        if (rc < 0) {
            printf("Test::connect:select: %d, %d, %s\n", rc, errno, strerror(errno));
            break;
        }
        if (0 == rc) {
            printf("Test::connect:time out: %d, %s\n", errno, strerror(errno));
            break;
        }
    } while (false);
    if (0 <= fd) {
        close(fd);
    }
    return ret;
}

int main(int argc, const char * argv[]) {
    getConnectState(80, "localhost", 1, 0);
    
    return 0;
}
