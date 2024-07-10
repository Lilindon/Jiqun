/*
muduo网络库给用户提供了两个主要的类：
TcpServer
TcpClient

epoll + 线程池
好处：将网络I/O代码 和 业务代码区分开
                       用户的连接和断开   用户的读写事件

bash:g++ testmuduo.cpp -lmuduo_net -lmuduo_base -lpthread -std=c++11
                        -I/usr/include 头文件路径
                        -L/usr/lib 库文件路径
                        -lmuduo_net 库文件名

relase版本 ：不包含调试信息 比较小
debug版本 ： -g 包含调试信息 比较大
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h> 
#include <iostream>
#include <functional> // 绑定器
#include <string>

using namespace std;
using namespace std::placeholders;
// 占位符：告诉调用回调函数时 要传的参数个数

// 基于muduo 开发server
// 1. 组合TcpServer对象
// 2. 创建EventLoop事件循环对象的指针
// 3. 明确TcpServer构造函数 需要什么参数，输出ChatServer
// 4. 在当前服务器类的构造函数中，注册处理连接的回调函数 与 处理读写事件的回调
// 5. 设置服务器端的线程数量 IO线程（连接） 与 工作线程（已有连接读写）
class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr,
            const string& nameArg)
            : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 回调函数：什么时候发生，不清楚，网络库监听是否发生，因此要设置回调
        // 普通函数：知道什么时候发生，指明道姓
        // 给服务器注册用户 连接的创建和断开回调
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        
        // 给服务器注册用户 读写事件回调
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
        
        // 设置服务器的线程数量： 1个IO 3个worker  线程池
        _server.setThreadNum(4);
    }

    void start()
    {
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开
    void onConnection(const muduo::net::TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {
            cout << conn->peerAddress().toIpPort() << " -> "<< conn->localAddress().toIpPort() << " stat::online" << endl;
        }
        else
        {
            cout << conn->peerAddress().toIpPort() << " -> "<< conn->localAddress().toIpPort() << " stat::offline" << endl;
            conn->shutdown(); // close(fd)
            // _loop->quit();
        }
    }

    void onMessage(const muduo::net::TcpConnectionPtr& conn, // 连接
                    muduo::net::Buffer* buffer,    // 缓冲区
                    muduo::Timestamp time)       // 接收到数据的时间信息
    {
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << " time:" <<  time.toString() << endl;
        conn->send(buf);
    }

    muduo::net::TcpServer _server;
    muduo::net::EventLoop * _loop; // epoll
};

int main()
{
    muduo::net::EventLoop loop; // epoll
    muduo::net::InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "Name::ChatServer");
    server.start(); // listenfd  epoll_ctl => epoll
    loop.loop(); // epoll_wait 以阻塞方式等待新用户连接、或已连接用户读写事件
    return 0;
}