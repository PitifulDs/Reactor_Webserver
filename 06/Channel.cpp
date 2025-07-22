#include "Channel.h"

Channel::Channel(Epoll *ep, int fd):ep_(ep), fd_(fd)
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


