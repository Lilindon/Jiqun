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
    // ע�����ӻص�  IO�߳� 
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
    // ע����Ϣ�ص�  ��д�߳� �������û��Ķ�д�ص�
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));
    // �����߳����� 4   һ����Reactor IO�߳� ����subReactor �����߳�
    _server.setThreadNum(4);
}

// ��������
void ChatServer::start()
{
    _server.start();
}

// �ϱ����������Ϣ�Ļص�
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{
    cout << " in onConnect()"<<endl;
    // �ͻ��˶Ͽ�����
    if(!conn->connected())
    {
        cout << " connection shutDown" <<endl;
        // ����ͻ����쳣�˳�
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// �ϱ���д�¼������Ϣ�Ļص�
void ChatServer::onMessage(const TcpConnectionPtr& conn,
                            Buffer *buffer,
                            Timestamp time)
{
    string buf = buffer->retrieveAllAsString();

    // ���ݷ����л�
    json js = json::parse(buf);
    
    // Ŀ�ģ���ȫ��������ģ��Ĵ��� �� ҵ��ģ��Ĵ���
    // js["msgId"]ֻ��json�е����� get������Ϊָ������
    auto msgHandler = ChatService::instance()->getHandler(js["msgId"].get<int>());
    // �ص���Ϣ�󶨺õ� �¼������� ��ִ����Ӧ��ҵ����
    msgHandler(conn, js, time);
}