#include "InetAddress.h"
#include "Socket.h"

// 创建非阻塞socket
int createnonblocking()
{
    int lisentfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK , IPPROTO_TCP);
    if (lisentfd < 0)
    {
        // perror("socket() error");exit(-1);
        printf("%s:%s:%d lisent socker create error:%d\n", __FILE__, __FUNCTION__, __LINE__, errno); exit(-1);
    }
    return lisentfd;
}


Socket::Socket(int fd):fd_(fd) // 构造函数,传入文件描述符
{

}

Socket::~Socket()   // 析构函数
{
    ::close(fd_);
}

int Socket::fd() const  // 获取文件描述符
{
    return fd_;
}
                             
std::string Socket::ip() const                // 返回ip_成员
{
    return ip_;
}
uint16_t Socket::port() const                // 返回port_成员
{
    return port_;
}
void Socket::setuseaddr(bool on)               // 设置SO_REUSEADDR选项， true表示开启，false表示关闭
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

}
void Socket::setreuseport(bool on) // 设置SO_REUSEPORT选项， true表示开启，false表示关闭
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

}
void Socket::settcpnodelay(bool on) // 设置TCP_NODELAY选项， true表示开启，false表示关闭
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, sizeof(opt));
}

void Socket::setkeepalive(bool on) // 设置SO_KEEPALIVE选项， true表示开启，false表示关闭
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
}

void Socket::bind(const InetAddress &servaddr) // 绑定地址,服务端的socket调用
{
    if(::bind(fd_, servaddr.addr(), sizeof(servaddr)) < 0)
    {
        perror("bind() error");close(fd_);exit(-1);
    }

    ip_ = servaddr.ip();
    port_ = servaddr.port();
}

void Socket::listen(int nn ) // 监听,服务端的socket调用
{
    if(::listen(fd_, nn) < 0)
    {
        perror("listen() error");close(fd_);exit(-1);
    }

}

int Socket::accept(InetAddress &clientaddr) // 获取客户端的连接,服务端的socket调用
{
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    int clintfd = ::accept4(fd_, (sockaddr*)&peeraddr, &len, SOCK_NONBLOCK);

    clientaddr.setaddr(peeraddr);

    ip_ = clientaddr.ip();
    port_ = clientaddr.port();

    return clintfd;
}