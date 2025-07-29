#include "TcpServer.h"


TcpServer::TcpServer(const std::string &ip, uint16_t port)
{
    // Socket *servsock = new Socket(createnonblocking()); // 这里new出来的对象没有释放，以后再说
    // InetAddress servaddr(ip, port); // 服务端的地址和协议
    // servsock->setuseaddr(true);
    // servsock->settcpnodelay(true);
    // servsock->setreuseport(true);
    // servsock->setkeepalive(true);
    // servsock->bind(servaddr);
    // servsock->listen();

    // EventLoop loop;
    // Channel *servchannel = new Channel(&loop, servsock->fd()); // 这里new出来的对象没有释放，以后再说
    // servchannel->setreadcallback(std::bind(&Channel::newconnection, servchannel, servsock));
    // servchannel->enablereading();
    acceptor_ = new Acceptor(&loop_, ip, port);
}

TcpServer::~TcpServer()
{
    delete acceptor_;
}

// 运行事件循环
void TcpServer::start()
{
    loop_.run();
}


