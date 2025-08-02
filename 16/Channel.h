#pragma once
#include <sys/epoll.h>
#include <functional>
#include "Epoll.h"
#include <sys/socket.h>
#include "InetAddress.h"
#include "Socket.h"
#include "EventLoop.h"

class  EventLoop;

class Channel{
private:
    int fd_ = -1;           // channel拥有的fd, channel和fd是一对一的关系
    // Epoll *ep_ = nullptr;   // channel对应的红黑树，channel与epoll是多对一的关系，一个channel只对应一个epoll
    EventLoop *loop_ = nullptr; // channel对应的事件循环,  channel与loop是多对一的关系, 一个channel只对应一个loop
    bool inepoll_ = false;  // fd是否在epoll中, 如果未添加，调用epoll_ctl()时候用EPOLL_CTL_ADD, 如果已添加，调用epoll_ctl()时候用EPOLL_CTL_MOD
    uint32_t events_ = 0;   // fd_需要监视的事件，listenfd和clientfd需要监视EPOLLIN， clientfdd 需要监视 EPOLLOUT
    uint32_t revents_ = 0;  // fd_实际发生的事件，EPOLLIN和EPOLLOUT
    bool islisten_ = false; // fd_是否是listenfd,客户端连上的fd取值为false
    std::function<void()> readcallback_;    // fd_读事件的回调函数

public:
    Channel(EventLoop *loop, int fd); // 构造函数 channel 是accptor和connection的下层类
    ~Channel(); // 析构函数

    int fd();                       // 返回fd_
    void useet();                   // 采用边缘触发
    void enablereading();           // 让epoll_wait()监视fd_的读事件
    void setinepoll();              // 把inepoll_成员的值设置为true；
    void setrevents(uint32_t ev);   // 设置revents_的值为参数ev
    bool inpoll();                  // 返回inepoll_的值
    uint32_t events();              // 返回events_的值
    uint32_t revents();             // 返回revents_的值

    void hanldleEvent();     // 事件处理函数，epoll_wait()返回时调用
    
    // void newconnection(Socket *servsock);    // 处理新客户端连接请求  转移至acceptor类中
    void onmessage();        // 处理对端发送过来的消息
    void setreadcallback(std::function<void()> fn); // 设置fd_读事件回调函数
};