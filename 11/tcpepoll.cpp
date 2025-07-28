/*
 * 程序名：tcpepoll.cpp，此程序用于演示采用epoll模型实现网络通讯的服务端。
 * 作者：吴从周
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>          
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>      // TCP_NODELAY需要包含这个头文件。
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
#include "EventLoop.h"
int main(int argc,char *argv[])
{
    if (argc != 3) 
    { 
        printf("usage: ./tcpepoll ip port\n");
        printf("example: ./tcpepoll 192.168.110.128 5058\n\n");
        return -1; 
    }

    Socket servsock(createnonblocking());
    InetAddress servaddr(argv[1],atoi(argv[2]));    // 服务端的地址和协议
    servsock.setuseaddr(true);
    servsock.settcpnodelay(true);
    servsock.setreuseport(true);
    servsock.setkeepalive(true);
    servsock.bind(servaddr);
    servsock.listen();

    EventLoop loop; 
    Channel *servchannel = new Channel(&loop, servsock.fd());
    servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel, &servsock));
    servchannel->enablereading();
    loop.run();

  return 0;
}