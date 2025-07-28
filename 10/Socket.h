#pragma once
#include "InetAddress.h"
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <netinet/tcp.h>


// 创建一个非阻塞的socket
int createnonblocking();



//Socket 类
class Socket{
private:
    const int fd_; // Socket持有的fd， 在构造函数中传进来  

public:
    Socket(int fd); // 构造函数
    ~Socket();  // 析构函数

    int fd() const; // 返回fd_成员
    void setuseaddr(bool on);   // 设置SO_REUSEADDR选项， true表示开启，false表示关闭
    void setreuseport(bool on); // 设置SO_REUSEPORT选项， true表示开启，false表示关闭
    void settcpnodelay(bool on); // 设置TCP_NODELAY选项， true表示开启，false表示关闭
    void setkeepalive(bool on); // 设置SO_KEEPALIVE选项， true表示开启，false表示关闭
    void bind(const InetAddress& servaddr); // 绑定地址,服务端的socket调用
    void listen(int nn = 128);              // 监听,服务端的socket调用
    int accept(InetAddress& clientaddr);   // 获取客户端的连接,服务端的socket调用 
};


