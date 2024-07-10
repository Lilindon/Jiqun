#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>

using namespace std;

// 处理服务器ctrl+c结束后 重置user的状态信息
void resetHandler(int){
    ChatService::instance()->reset();
    exit(0);
}

int main()
{
    signal(SIGINT, resetHandler); // 服务器 ctrl+c 异常终止后 将 online 变为 offline
    muduo::net::EventLoop loop; // epoll
    muduo::net::InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "Name::ChatServer");
    server.start(); // listenfd  epoll_ctl => epoll
    loop.loop(); // epoll_wait 以阻塞方式等待新用户连接、或已连接用户读写事件
    return 0; 
}