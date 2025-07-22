#include "Epoll.h"




Epoll::Epoll()
{
    if( (epollfd_ = epoll_create(1)) == -1 ) // 创建epoll句柄（红黑树）。
    {
        printf("epoll_create error(%d).\n", errno);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    close(epollfd_);
}

void Epoll::addfd(int fd, uint32_t op)
{
    //为服务端的listenfd准备读事件。
    epoll_event ev;         // 声明事件的数据结构。
    ev.data.fd = fd; // 指定事件的自定义数据，会随着epoll_wait()返回的事件一并返回。
    ev.events = op;        // 让epoll监视listenfd的读事件，采用水平触发。

    if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev)) // 把需要监视的listenfd和它的事件加入epollfd中。
    {
        printf("epoll_ctl error(%d)\n", errno);
        exit(-1);
    }
}

std::vector<epoll_event> Epoll::loop(int timeout)  // 运行epoll_wait(), 等待事件的发生，已发生的事件用vector容器返回
{
    std::vector<epoll_event> evs;   // 存放epoll_wait()返回的已发生事件。
    
    bzero(events_, sizeof(events_));
    int infds = epoll_wait(epollfd_, events_, MaxEvents, timeout); // 等待监视的fd有事件发生。

    // 返回失败。
    if (infds < 0)
    {
        perror("epoll_wait() failed");
        exit(-1);
    }

    // 超时。
    if (infds == 0)
    {
        printf("epoll_wait() timeout.\n");
        return evs;
    }

    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii = 0; ii < infds; ii++) // 遍历epoll返回的数组evs。
    {
        evs.push_back(events_[ii]);
    }
    return evs;
}