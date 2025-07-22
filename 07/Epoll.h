#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include "Channel.h"

// Epoll 类
class Epoll{

private:
    static const int MaxEvents = 1000;  // 最大事件数
    int epollfd_ = -1;   // epoll 文件描述符, 在构造函数中创建
    epoll_event events_[MaxEvents]; // 存放epoll_wait返回时间的数组，在构造函数中分配内存

public:
    Epoll();    // 构造函数
    ~Epoll();   // 析构函数

    // void addfd(int fd, uint32_t op);    // 把fd和它需要坚实的时间添加到红黑树上
    void updatechannel(Channel *ch); // 把channel添加/更新到红黑树上，channel中有fd，也有需要监视的事件
    // std::vector<epoll_event> loop(int timeout = -1);    // 运行epoll_wait(), 等待事件的发生，已发生的事件用vector容器返回
    std::vector<Channel *> loop(int timeout = -1); // 运行epoll_wait(), 等待事件的发生，已发生的事件用vector容器返回
};