#include "TcpServer.h"


TcpServer::TcpServer(const std::string &ip, uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->setnewconnectioncb(std::bind(&TcpServer::newconnection, this, std::placeholders::_1));
}

TcpServer::~TcpServer()
{
    delete acceptor_;
}

// 运行事件循环
void TcpServer::start()
{
    loop_.run();
}

void TcpServer::newconnection(Socket *clientsock) // 处理新客户端连接请求
{
    Connection *conne = new Connection(&loop_, clientsock); // 这里new出来没有释放，以后再解决
}