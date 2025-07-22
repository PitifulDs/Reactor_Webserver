#include "Epoll.h"




Epoll::Epoll()
{
    if( epollfd_ = epoll_create(1)) // 创建epoll句柄（红黑树）。
    {
        printf("epoll_create error(%d).\n", errno);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    close(epollfd_);
}

/*
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
*/

// 把channel添加/更新到红黑树上，channel中有fd，也有需要监视的事件
void Epoll::updatechannel(Channel *ch) 
{
    epoll_event ev;             // 定义epoll_event结构体变量，用于注册事件
    ev.data.ptr = ch;           // 指定channel
    ev.events = ch->events();   // 获取channel中需要监听的事件

    if(ch->inpoll())    // channel是否在epoll树中
    {
        if(epoll_ctl(epollfd_, EPOLL_CTL_MOD, ch->fd(), &ev) == -1)
        {
            perror("epoll_ctl() failed.\n");
            exit(-1);
        }
        
    }else{
        if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->fd(), &ev) == -1)
        {
            perror("epoll_ctl() failed.\n");
            exit(-1);
        }
        ch->setinepoll();   // 设置channel在epoll树中
    }


}

// std::vector<epoll_event> Epoll::loop(int timeout)  // 运行epoll_wait(), 等待事件的发生，已发生的事件用vector容器返回
// {
//     std::vector<epoll_event> evs;   // 存放epoll_wait()返回的已发生事件。
    
//     bzero(events_, sizeof(events_));
//     int infds = epoll_wait(epollfd_, events_, MaxEvents, timeout); // 等待监视的fd有事件发生。

//     // 返回失败。
//     if (infds < 0)
//     {
//         perror("epoll_wait() failed");
//         exit(-1);
//     }

//     // 超时。
//     if (infds == 0)
//     {
//         printf("epoll_wait() timeout.\n");
//         return evs;
//     }

//     // 如果infds>0，表示有事件发生的fd的数量。
//     for (int ii = 0; ii < infds; ii++) // 遍历epoll返回的数组evs。
//     {
//         evs.push_back(events_[ii]);
//     }
//     return evs;
// }

std::vector<Channel *> Epoll::loop(int timeout) // 运行epoll_wait(), 等待事件的发生，已发生的事件用vector容器返回
{
    std::vector<Channel *> channels; // 存放epoll_wait()返回的已发生事件。

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
        return channels;
    }

    // 如果infds>0，表示有事件发生的fd的数量。
    for (int ii = 0; ii < infds; ii++) // 遍历epoll返回的数组evs。
    {
        // evs.push_back(events_[ii]);
        Channel *ch = static_cast<Channel *>(events_[ii].data.ptr); // 取出已发生时间的channel
        ch->setrevents(events_[ii].events); // 设置channel的revents_成员。
        channels.push_back(ch);
    }
    return channels;
}