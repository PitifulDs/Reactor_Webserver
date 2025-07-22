#include "Channel.h"


Channel::Channel(Epoll *ep, int fd, bool islisten):ep_(ep), fd_(fd), islisten_(islisten)
{

}

Channel::~Channel()
{
    // 在析构函数中，不要销毁ep_，也不能关闭fd_，因为这两不属于channel类，channel类仅仅使用他们 
}

int Channel::fd()
{
    return fd_;
}

void Channel::useet()
{
    events_ = events_ | EPOLLET;
}

void Channel::enablereading()   // 让epoll_wait()监听fd_的读事件
{
    events_ = events_ | EPOLLIN;
    ep_->updatechannel(this); // this 表示当前 channel 对象自己，作为参数传递给 updatechannel 方法。
}

void Channel::setinepoll()
{
    inepoll_ = true;
}

void Channel::setrevents(uint32_t ev)
{
    revents_ = ev;

}

bool Channel::inpoll()
{
    return inepoll_;
}

uint32_t Channel::events()
{
    return events_;
}   

uint32_t Channel::revents()
{
    return revents_;
}

void Channel::hanldleEvent(Socket *servsock) // 事件处理函数，epoll_wait()返回时调用
{
    if (revents_ & EPOLLRDHUP) // 对方已关闭，有些系统检测不到，可以使用EPOLLIN，recv()返回0。
    {
        printf("client(eventfd=%d) disconnected.\n", fd_);
        close(fd_); // 关闭客户端的fd。
    } //  普通数据  带外数据
    else if (revents_ & (EPOLLIN | EPOLLPRI)) // 接收缓冲区中有数据可以读。
    {
        if (islisten_ == true) // 如果是listenfd有事件，表示有新的客户端连上来。
        {
            InetAddress clientaddr; // 客户端的地址和协议
            // 注意，clientsock只能new出来，不能再栈上，否则析构函数会关闭fd
            // 这里new出来没有释放，以后再解决
            Socket *clientsock = new Socket(servsock->accept(clientaddr));

            printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", clientsock->fd(), clientaddr.ip(), clientaddr.port());

            // 为新客户端连接准备读事件，并添加到epoll中。
            Channel *clientchannel = new Channel(ep_, clientsock->fd(), false);
            clientchannel->useet();
            clientchannel->enablereading();

            ////////////////////////////////////////////////////////////////////////
        }
        else // 如果是客户端连接的fd有事件。
        {
            ////////////////////////////////////////////////////////////////////////
            char buffer[1024];
            while (true) // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕。
            {
                bzero(&buffer, sizeof(buffer));
                ssize_t nread = read(fd_, buffer, sizeof(buffer));
                if (nread > 0) // 成功的读取到了数据。
                {
                    // 把接收到的报文内容原封不动的发回去。
                    printf("recv(eventfd=%d):%s\n", fd_, buffer);
                    send(fd_, buffer, strlen(buffer), 0);
                }
                else if (nread == -1 && errno == EINTR) // 读取数据的时候被信号中断，继续读取。
                {
                    continue;
                }
                else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) // 全部的数据已读取完毕。
                {
                    break;
                }
                else if (nread == 0) // 客户端连接已断开。
                {
                    printf("client(eventfd=%d) disconnected.\n", fd_);
                    close(fd_); // 关闭客户端的fd。
                    break;
                }
            }
        }
    }
    else if (revents_ & EPOLLOUT) // 有数据需要写，暂时没有代码，以后再说。
    {
    }
    else // 其它事件，都视为错误。
    {
        printf("client(eventfd=%d) error.\n", fd_);
        close(fd_); // 关闭客户端的fd。
    }
}


