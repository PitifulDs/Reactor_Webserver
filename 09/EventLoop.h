#pragma once
#include "Epoll.h"



// 事件循环类
class EventLoop
{
private:
    Epoll *ep_;  // 每个事件循环只有一个Epoll

public:
    EventLoop();    // 构造函数
    ~EventLoop();   // 析构函数

    void run(); //  运行事件循环
    Epoll *ep();    // 返回epoll对象的地址
};






