#include "InetAddress.h"

InetAddress::InetAddress(const std::string &ip, uint16_t port) // 如果是监听的fd，用这个构造函数
{
    addr_.sin_family = AF_INET; // ipv4网络协议的套接字类型
    addr_.sin_port = htons(port); // 服务端用于监听的端口
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());  // ip地址
}

InetAddress::InetAddress(const sockaddr_in addr):addr_(addr)    // 如果是客户端连接的fd，用这个构造函数
{   
    
}

// InetAddress::~InetAddress()
// {
    
// }

// 获取IP地址的函数
// 返回: 指向包含IP地址的字符串的常量指针
const char *InetAddress::ip() const
{
    return inet_ntoa(addr_.sin_addr);
}

// 获取端口号的函数
// 返回: 无符号16位整数，表示端口号
uint16_t InetAddress::port() const
{
    return ntohs(addr_.sin_port);
}

// 获取地址信息的函数
// 返回: 指向 sockaddr 结构体的常量指针，包含地址信息
const sockaddr *InetAddress::addr() const // 在bind函数中是sockaddr结构体，所以这里是sockaddr*
{
    return (sockaddr*)&addr_;
}

