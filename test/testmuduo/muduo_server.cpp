/*
muduo�������û��ṩ��������Ҫ���ࣺ
TcpServer
TcpClient

epoll + �̳߳�
�ô���������I/O���� �� ҵ��������ֿ�
                       �û������ӺͶϿ�   �û��Ķ�д�¼�

bash:g++ testmuduo.cpp -lmuduo_net -lmuduo_base -lpthread -std=c++11
                        -I/usr/include ͷ�ļ�·��
                        -L/usr/lib ���ļ�·��
                        -lmuduo_net ���ļ���

relase�汾 ��������������Ϣ �Ƚ�С
debug�汾 �� -g ����������Ϣ �Ƚϴ�
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h> 
#include <iostream>
#include <functional> // ����
#include <string>

using namespace std;
using namespace std::placeholders;
// ռλ�������ߵ��ûص�����ʱ Ҫ���Ĳ�������

// ����muduo ����server
// 1. ���TcpServer����
// 2. ����EventLoop�¼�ѭ�������ָ��
// 3. ��ȷTcpServer���캯�� ��Ҫʲô���������ChatServer
// 4. �ڵ�ǰ��������Ĺ��캯���У�ע�ᴦ�����ӵĻص����� �� �����д�¼��Ļص�
// 5. ���÷������˵��߳����� IO�̣߳����ӣ� �� �����̣߳��������Ӷ�д��
class ChatServer
{
public:
    ChatServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr,
            const string& nameArg)
            : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // �ص�������ʲôʱ����������������������Ƿ��������Ҫ���ûص�
        // ��ͨ������֪��ʲôʱ������ָ������
        // ��������ע���û� ���ӵĴ����ͶϿ��ص�
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
        
        // ��������ע���û� ��д�¼��ص�
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
        
        // ���÷��������߳������� 1��IO 3��worker  �̳߳�
        _server.setThreadNum(4);
    }

    void start()
    {
        _server.start();
    }

private:
    // ר�Ŵ����û������Ӵ����ͶϿ�
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

    void onMessage(const muduo::net::TcpConnectionPtr& conn, // ����
                    muduo::net::Buffer* buffer,    // ������
                    muduo::Timestamp time)       // ���յ����ݵ�ʱ����Ϣ
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
    loop.loop(); // epoll_wait ��������ʽ�ȴ����û����ӡ����������û���д�¼�
    return 0;
}