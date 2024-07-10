#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <functional>
#include <string>
#include <iostream>

using namespace std;
using namespace std::placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册链接回调  IO线程 
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // 注册消息回调  读写线程 已连接用户的读写回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    // 设置线程数量 4   一个主Reactor IO线程 三个subReactor 工作线程
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
    _server.start();
}

// 上报链接相关信息的回调
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    cout << " in onConnect()"<<endl;
    // 客户端断开连接
    if(!conn->connected())
    {
        cout << " connection shutDown" <<endl;
        // 处理客户端异常退出
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调
void ChatServer::onMessage(const TcpConnectionPtr& conn,
                            Buffer *buffer,
                            Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // 数据反序列化
    json js = json::parse(buf);
    
    // 目的：完全解耦网络模块的代码 和 业务模块的代码
    // js["msgId"]只是json中的类型 get方法变为指定类型
    auto msgHandler = ChatService::instance()->getHandler(js["msgId"].get<int>());
    // 回调消息绑定好的 事件处理器 来执行相应的业务处理
    msgHandler(conn, js, time);
}