#include "Acceptor.h"
#include "Connection.h"
Acceptor::Acceptor(EventLoop *loop, const std::string& ip, const uint16_t port):loop_(loop)
{
    servsock_ = new Socket(createnonblocking()); 
    InetAddress servaddr(ip, port);                     // 服务端的地址和协议
    servsock_->setuseaddr(true);
    servsock_->settcpnodelay(true);
    servsock_->setreuseport(true);
    servsock_->setkeepalive(true);
    servsock_->bind(servaddr);
    servsock_->listen();

    acceptchannel_ = new Channel(loop_, servsock_->fd());
    // acceptchannel_->setreadcallback(std::bind(&Channel::newconnection, acceptchannel_, servsock_));
    acceptchannel_->setreadcallback(std::bind(&Acceptor::newconnection, this));
    acceptchannel_->enablereading();
}



Acceptor::~Acceptor()
{ 
    delete servsock_;
    delete acceptchannel_;
}

void Acceptor::newconnection() // 处理新客户端连接请求
{
    InetAddress clientaddr; // 客户端的地址和协议
    // 注意，clientsock只能new出来，不能再栈上，否则析构函数会关闭fd
    // 这里new出来没有释放，以后再解决
    Socket *clientsock = new Socket(servsock_->accept(clientaddr));

    clientsock->setipport(clientaddr.ip(), clientaddr.port());
    newconnectioncb_(clientsock);
}

void Acceptor::setnewconnectioncb(std::function<void(Socket *)> fn) // 设置处理新客户端连接请求的回调函数
{
    newconnectioncb_ = fn;
}
