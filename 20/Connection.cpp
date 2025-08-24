#include "Connection.h"

Connection::Connection(EventLoop *loop, Socket *clientsock):loop_(loop), clientsock_(clientsock)
{
    // 为新客户端连接准备读事件，并添加到epoll中。
    clientchannel_ = new Channel(loop_, clientsock->fd()); // 这里new出来没有释放，以后再解决
    // clientchannel_->setreadcallback(std::bind(&Channel::onmessage, clientchannel_));
    clientchannel_->setreadcallback(std::bind(&Connection::onmessage, this));
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

void Connection::onmessage() // 处理对端发送过来的消息
{
    char buffer[1024];
    while (true) // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
    {
        bzero(&buffer, sizeof(buffer));
        ssize_t nread = read(fd(), buffer, sizeof(buffer));
        if (nread > 0) // 成功的读取到了数据。
        {
            // 把接收到的报文内容原封不动的发回去。
            // printf("recv(eventfd=%d):%s\n", fd(), buffer);
            // send(fd(), buffer, strlen(buffer), 0);
            inputbuffer_.append(buffer, nread);     // 把读取的数据追加到接收缓冲区中
        }
        else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
        {
            continue;
        }
        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
        {
            printf("recv(eventfd=%d):%s\n", fd(), inputbuffer_.data());
            // 在这里，将经过若干步骤的运算
            outputbuffer_ = inputbuffer_;
            // 运算后的结果已存放在outputbuffer_中
            inputbuffer_.clear();           // 清空接收缓冲区
            send(fd(), outputbuffer_.data(), outputbuffer_.size(), 0);      // 发送数据
            break;
        }
        else if (nread == 0) // 客户端连接已断开。
        {
            // printf("client(eventfd=%d) disconnected.\n", fd_);
            // close(fd_); // 关闭客户端的fd。
            closecallback();
            break;
        }
    }
}

void Connection::closecallback() // TCP连接关闭/断开得回调函数，供channel回调
{
    // printf("client(eventfd=%d) disconnected.\n", fd());
    // close(fd()); // 关闭客户端的fd。
    closecallback_(this);   // 回调TcpServer::closeconnection
}

void Connection::errorcallback() // TCP连接出错的回调函数，供channel回调
{
    // printf("client(eventfd=%d) error.\n", fd());
    // close(fd()); // 关闭客户端的fd。
    errorcallback_(this); // 回调TcpServer::errorconnection
}

void Connection::setclosecallback(std::function<void(Connection *)> fn) // 设置关闭fd_的回调函数
{
    closecallback_ = fn;
}

void Connection::seterrorcallback(std::function<void(Connection *)> fn) // 设置错误fd_的回调函数
{
    errorcallback_ = fn;
}