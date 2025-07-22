#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>


// socket的地址协议类
class InetAddress{
    private:
        sockaddr_in addr_; // socket地址结构体
    public:
        InetAddress();
        InetAddress(const std::string &ip, uint16_t port);  // 如果是监听的fd，用这个构造函数
        InetAddress(const sockaddr_in addr);  // 如果是客户端连上的fd，用这个构造函数
        ~InetAddress() = default;

        // 获取IP地址的函数
        // 返回: 指向包含IP地址的字符串的常量指针
        const char* ip() const;
        
        // 获取端口号的函数
        // 返回: 无符号16位整数，表示端口号
        uint16_t port() const;
        
        // 获取地址信息的函数
        // 返回: 指向 sockaddr 结构体的常量指针，包含地址信息
        const sockaddr* addr() const; // 在bind函数中是sockaddr结构体，所以这里是sockaddr*
        void setaddr(sockaddr_in clientaddr); // 设置addr_成员的值
};