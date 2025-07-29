#include "TcpServer.h"


TcpServer::TcpServer(const std::string &ip, uint16_t port)
{
    Socket *servsock = new Socket(createnonblocking());
    InetAddress servaddr(ip, port); // 服务端的地址和协议
    servsock->setuseaddr(true);
    servsock->settcpnodelay(true);
    servsock->setreuseport(true);
    servsock->setkeepalive(true);
    servsock->bind(servaddr);
    servsock->listen();

    EventLoop loop;
    Channel *servchannel = new Channel(&loop, servsock->fd());
    servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel, servsock));
    servchannel->enablereading();
}

TcpServer::~TcpServer()
{

}

// 运行事件循环
void TcpServer::start()
{
    loop_.run();
}


