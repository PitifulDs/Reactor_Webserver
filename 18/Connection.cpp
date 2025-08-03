#include "Connection.h"

Connection::Connection(EventLoop *loop, Socket *clientsock):loop_(loop), clientsock_(clientsock)
{
    // 为新客户端连接准备读事件，并添加到epoll中。
    clientchannel_ = new Channel(loop_, clientsock->fd()); // 这里new出来没有释放，以后再解决
    clientchannel_->setreadcallback(std::bind(&Channel::onmessage, clientchannel_));
    clientchannel_->setclosecallback(std::bind(&Connection::closecallback, this));
    clientchannel_->seterrorcallback(std::bind(&Connection::errorcallback, this));
    clientchannel_->useet();
    clientchannel_->enablereading();
}

Connection::~Connection()
{
    delete clientsock_;
    delete clientchannel_;
}

int Connection::fd() const // 返回客户端fd_
{
    return clientsock_->fd();
}
std::string Connection::ip() const // 返回客户端ip_
{
    return clientsock_->ip();
}
uint16_t Connection::port() const  // 返回客户端port_
{
    return clientsock_->port();
}

void Connection::closecallback() // TCP连接关闭/断开得回调函数，供channel回调
{
    printf("client(eventfd=%d) disconnected.\n", fd());
    close(fd()); // 关闭客户端的fd。
}

void Connection::errorcallback() // TCP连接出错的回调函数，供channel回调
{
    printf("client(eventfd=%d) error.\n", fd());
    close(fd()); // 关闭客户端的fd。
}
