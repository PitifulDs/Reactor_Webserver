#include "TcpServer.h"


TcpServer::TcpServer(const std::string &ip, uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
    delete acceptor_;
    for(auto& aa : conns_)
    {
        delete aa.second;
    }
}

// 运行事件循环
void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newconnection(Socket *clientsock) // 处理新客户端连接请求
{
    Connection *conn = new Connection(&loop_, clientsock); // 这里new出来没有释放，以后再解决
    conn->setclosecallback(std::bind(&TcpServer::closeconnection, this, std::placeholders::_1));
    conn->seterrorcallback(std::bind(&TcpServer::errorconnection, this, std::placeholders::_1));
    conn->setonmessagecallback(std::bind(&TcpServer::onmessage, this, std::placeholders::_1, std::placeholders::_2));
    printf("new connection (fd=%d,ip=%s,port=%d) ok.\n", conn->fd(), conn->ip().c_str(), conn->port());

    conns_[conn->fd()] = conn;  // 保存连接

}

void TcpServer::closeconnection(Connection *conn) // 处理客户端断开连接, 在connection类中调用
{
    printf("client(eventfd=%d) disconnected.\n", conn->fd());
    // close(conn->fd()); // 关闭客户端的fd。在socket类中已经处理了
    conns_.erase(conn->fd());   // 删除连接
    delete conn;
}

void TcpServer::errorconnection(Connection *conn) // 处理客户端连接错误，在connection类中调用
{
    printf("client(eventfd=%d) error.\n", conn->fd());
    // close(conn->fd()); // 关闭客户端的fd。在socket类中已经处理了
    conns_.erase(conn->fd()); // 删除连接
    delete conn;
}

void TcpServer::onmessage(Connection *conn, std::string message) // 处理客户端发来的消息，在connection类中调用
{
    // 在这里，将经过若干步骤的运算
    message = "reply: " + message;

    int len = message.size();                // 计算回应报文的大小
    std::string tmpbuf((char *)&len, 4); // 把报文头部填充到回应报文中
    tmpbuf.append(message);              // 把报文内容填充到回应报文中

    send(conn->fd(), tmpbuf.data(), tmpbuf.size(), 0); // 发送数据
}