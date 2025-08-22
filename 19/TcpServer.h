#pragma once
#include "Channel.h"
#include "Socket.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Connection.h"
#include <map>
// TCP网络服务类
class TcpServer
{
private:
    EventLoop loop_;        // 一个TcpServer可以有多个事件循环，现在是单线程，暂时只用一个事件循环  
    Acceptor *acceptor_;    // 一个TcpSever只有一个Acceptor对象
    std::map<int, Connection *> conns_; // 一个TcpServer可以有多个connection对象，存放在map容器中
public:
    TcpServer(const std::string &ip, const uint16_t port);
    ~TcpServer();

    void start();   // 运行事件循环
    void newconnection(Socket *clientsock); // 处理新客户端连接请求
    void closeconnection(Connection *conn); // 处理客户端断开连接, 在connection类中调用
    void errorconnection(Connection *conn); // 处理客户端连接错误，在connection类中调用
};

