#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);

    void start();
private:
    // IO线程 监控链接创建断开回调
    void onConnection(const TcpConnectionPtr&);
    // Work线程 监控已有链接的读写操作
    void onMessage(const TcpConnectionPtr&,
                        Buffer*,
                        Timestamp);

    TcpServer _server;
    EventLoop*  _loop;
};

#endif