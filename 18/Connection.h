#pragma once
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "EventLoop.h"
#include <functional>

class Connection
{
private:
    EventLoop *loop_;        // Connection对应得事件循环，在构造函数中传入
    Socket *clientsock_;       // 服务端用于监听得socket，在构造函数中创建
    Channel *clientchannel_; // Connection用于监听的Channel，在构造函数中创建
public:
    Connection(EventLoop *loop, Socket *clientsock);
    ~Connection();

    int fd() const;         // 返回客户端fd_
    std::string ip() const; // 返回客户端ip_
    uint16_t port() const;  // 返回客户端port_

    void closecallback();   // TCP连接关闭/断开得回调函数，供channel回调
    void errorcallback();   // TCP连接出错的回调函数，供channel回调

};
