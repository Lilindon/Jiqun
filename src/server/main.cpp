#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>

using namespace std;

// ���������ctrl+c������ ����user��״̬��Ϣ
void resetHandler(int){
    ChatService::instance()->reset();
    exit(0);
}

int main()
{
    signal(SIGINT, resetHandler); // ������ ctrl+c �쳣��ֹ�� �� online ��Ϊ offline
    muduo::net::EventLoop loop; // epoll
    muduo::net::InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "Name::ChatServer");
    server.start(); // listenfd  epoll_ctl => epoll
    loop.loop(); // epoll_wait ��������ʽ�ȴ����û����ӡ����������û���д�¼�
    return 0; 
}