#ifndef CHATSERVICE_H  // ��ֹͷ�ļ��ظ�����
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>

#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;

// c++11 �﷨ �����µ���������
// ������Ϣ���¼��ص���������
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// ���������ҵ����
class ChatService
{
public:
    // �����½ҵ��
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����ע��ҵ��
    void loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����ע��ҵ��
    void registe(const TcpConnectionPtr &conn, json &js, Timestamp time);
    
    // ��ȡ��Ϣ��Ӧ�Ĵ�����
    MsgHandler getHandler(int msgId);
    // ����ͻ����쳣�˳�
    void clientCloseException(const TcpConnectionPtr &conn);
    // ����������쳣�˳���ҵ�����÷���
    void reset();
    // һ��һ����ҵ��
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ��Ӻ���ҵ��
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����Ⱥ��ҵ��
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ���Ⱥ��ҵ��
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // Ⱥ������ҵ��
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void handleRedisSubscribeMessage(int userid, string msg);
    // ��ȡ��������Ľӿں���
    static ChatService* instance();

private:
    // ����ģʽ��1.���캯�� ˽�л� 2.дһ��Ωһ��ʵ�� 3.�ٱ�¶һ���ӿ�
    ChatService();

    // �洢��Ϣid ����ҵ������
    unordered_map<int, MsgHandler> _msgHandlerMap;
    
    // �洢�����û���ͨ������ (���ʶ࣬��д��ע���̰߳�ȫ)
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // ���廥���� ��֤_userConnMap���̰߳�ȫ
    mutex _connMutex;

    // ���ݲ��������
    UserModel _userModel;

    // ������Ϣ��������  
    OfflineMsgModel _offlineMsgModel;

    // ��Ӻ��������
    FriendModel _fridneModel;

    // �������
    GroupModel _groupModel;

    // Redis��������
    Redis _redis;
};

#endif