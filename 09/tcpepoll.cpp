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

    Epoll ep;
    // ep.addfd(servsock.fd(), EPOLLIN);   // 添加listenfd到epollfd中。
    Channel *servchannel = new Channel(&ep, servsock.fd(), true);
    servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel, &servsock));
    servchannel->enablereading();
    // std::vector<epoll_event> evs;   // 存放epoll_wait()返回的事件。

    while (true)        // 事件循环。
    {
        // std::vector<epoll_event> evs; // 存放epoll_wait()返回的事件。
        std::vector<Channel *> channels; // 存放epoll_wait()返回的事件。
        channels = ep.loop();            // 等待监视的fd有事件发生

        // 如果infds>0，表示有事件发生的fd的数量。
        // for (int ii=0;ii<infds;ii++)       // 遍历epoll返回的数组evs。
        for (auto &ch : channels)
        {
             ch->hanldleEvent();
        }
    }

  return 0;
}