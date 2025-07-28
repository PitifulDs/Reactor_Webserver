#include "EventLoop.h"

EventLoop::EventLoop():ep_(new Epoll)
{
    
}

EventLoop::~EventLoop()
{
    delete ep_;
}

void EventLoop::run()
{
    while (true) // 事件循环。
    {
        std::vector<Channel *> channels; // 存放epoll_wait()返回的事件。
        channels = ep_->loop();            // 等待监视的fd有事件发生

        for (auto &ch : channels)
        {
            ch->hanldleEvent(); // 处理epoll_wait()返回的事件
        }
    }
}


Epoll* EventLoop::ep()
{
    return ep_;
}
