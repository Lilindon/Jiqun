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

// int main()
// {
//     signal(SIGINT, resetHandler); // ������ ctrl+c �쳣��ֹ�� �� online ��Ϊ offline
//     muduo::net::EventLoop loop; // epoll
//     muduo::net::InetAddress addr("127.0.0.1", 6000);
//     ChatServer server(&loop, addr, "Name::ChatServer");
//     server.start(); // listenfd  epoll_ctl => epoll
//     loop.loop(); // epoll_wait ��������ʽ�ȴ����û����ӡ����������û���д�¼�
//     return 0; 
// }

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // ����ͨ�������в������ݵ�ip��port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}