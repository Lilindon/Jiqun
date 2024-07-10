#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace muduo;

// ��̬����������ʵ�� ����дstatic
ChatService* ChatService::instance()
{
    // �������� �̰߳�ȫ
    static ChatService service;
    return &service;
}

// ע����Ϣ�Լ���Ӧ��Handler�ص�
ChatService::ChatService()
{
    // ��½
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    // ע��
    _msgHandlerMap.insert({REGISTE_MSG, std::bind(&ChatService::registe, this, _1, _2, _3)});
    // һ��һ�Ի�
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    // ��Ӻ����б�
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    // ����Ⱥ��
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    // _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    // _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
}


// ��ȡ��Ϣ��Ӧ�Ĵ�����
MsgHandler ChatService::getHandler(int msgId)
{
    // ��¼������־��msgIdû�ж�Ӧ���¼�������
    auto it = _msgHandlerMap.find(msgId); // ������[] []�����������һ��
    if(it == _msgHandlerMap.end())
    { 
        // muduo��־ #include <muduo/base/Logging.h>
        // ����һ��Ĭ�ϵĴ��������ղ���
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp){
            LOG_ERROR << "msgId: " << msgId << " can not find handler! ";
        };
    }
    else
    {
        return _msgHandlerMap[msgId];
    }
}

// �����½ҵ��   ORM  object relation map  �����ϵӳ��  ҵ�������Ķ��Ƕ���   ҵ��ģ��  ����ģ��  ����
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"].get<int>(); // ��id��Ϊ����ʱ�����еĲ���
    string password = js["password"];

    User user = _userModel.query(id);
    if(user.getId() == id && user.getPwd() == password){
        if(user.getState() == "online"){
            // ���û��ѵ�½ �������ظ���½
            json response;
            response["msgId"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "user are inlined please reinsert";
            conn->send(response.dump()); // ���л�����
        }
        else{
            // ��½�ɹ�, ��¼�û�������Ϣ(���̷߳��� Ҫע���̰߳�ȫ����)
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({user.getId(), conn});
            }

             // �����û���½״̬ state:: offline->online
            user.setState("online"); 
            _userModel.updateState(user); // _userModel�������ݿ�

            // ��½�ɹ�������Ϣ
            json response;
            response["msgId"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            
            // ��ѯ���û��Ƿ���������Ϣ ������� ������Ϣ���������ݿ���
            vector<string > vec = _offlineMsgModel.query(id);
            if(!vec.empty()){
                response["offlinemsg"] = vec;
                // ��¼������Ϣ��json�� �����ݿ���ɾ��������Ϣ
                _offlineMsgModel.drop(id);
            }

            // ��ѯ�û��ĺ�����Ϣ �����ظ��ͻ��� js.dump
            vector<User> userVec = _fridneModel.query(id);
            if(!userVec.empty()){
                vector<string> vec2;
                for(User &user : userVec){
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            conn->send(response.dump()); // ���л�����
        }
    }else{
        // �û������ڵ�½ʧ�ܣ��û����ڵ�������󣬵�½ʧ��
        json response;
        response["msgId"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "username Or password error";
        conn->send(response.dump()); // ���л�����
    }
}

// ����ע��ҵ�� �����л�json�ļ��е� name password
void ChatService::registe(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    string name = js["name"];
    string pwd = js["password"];

    // User���󴴽�  ҵ���в����Ķ������ݶ��� ���漰���ݿ��ֶ�
    User user;
    user.setName(name);
    user.setPwd(pwd);
    // ���ݿ����
    bool state = _userModel.insert(user);
    if(state){
        // ע��ɹ� ��Ӧ json
        json response;
        response["msgId"] = REGISTE_MSG_ACK; 
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump()); // json���л�����
    }else{
        // ע��ʧ��
        json response;
        response["msgId"] = REGISTE_MSG_ACK;
        response["errno"] = 1;
        // response["errmsg"] = REGISTE_MSG_FAILED;
        response["id"] = user.getId();
        conn->send(response.dump()); // json���л�����
    }
}

// ����ͻ����쳣�˳�
void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    User user;
    // �����쳣�˳�֮ǰ����֤_userConnMap���̰߳�ȫ���� ͬʱ�����쳣��loginʱ��map
    {
        lock_guard<mutex> lock(_connMutex);
        for(auto it=_userConnMap.begin(); it!=_userConnMap.end(); ++it)
        {
            if(it->second == conn) // ��ѯ��conn������ָ��
            {
                // ��unordered_map<int, conn>ɾ�����Ӷ�
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1){  // ��Ϊ-1 ���id���û�������
        // �����û�״̬��Ϣ
        user.setState("offline");
        _userModel.updateState(user); // ���ݿ��޸�_userModel
    }
}

// �������쳣��ҵ�����÷���
void ChatService::reset(){
    // ���ݿ������usermodel��online״̬���û������ó�offline
    _userModel.resetState();
}

// һ��һ����ҵ��
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int toId = js["toid"].get<int>();

    {
        // �̰߳�ȫ_userConnMap ������Ϣ��
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toId);
        if(it != _userConnMap.end()){
            // toId ���� ת����Ϣ  it->secondָ�� toId��Conn
            // server����������Ϣ�� toId�û�
            it->second->send(js.dump());
            return;
        }
    }
    // toId ������ �洢������Ϣ   ������Ϣ��offlineMsgModel  ��������Ϣ�洢�����ݿ���
    _offlineMsgModel.insert(toId, js.dump());
}

// Ⱥ������ҵ��
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    // ��ȡȺ��ת�������б�
    vector<int> useridVec = _groupModel.queryGroupUsers(userid, groupid);
    // connmap����̰߳�ȫ
    lock_guard<mutex> lock(_connMutex);
    for(int id : useridVec){
        auto it =  _userConnMap.find(id);
        if(it != _userConnMap.end()){
            // �����û�������ֱ��ת����Ϣ
            it->second->send(js.dump());
        }else{
            // �����û������� ��洢������Ϣ
            _offlineMsgModel.insert(id, js.dump());
        }
    }
}


// ��Ӻ���ҵ��
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userId = js["userid"].get<int>();
    int friendId = js["friendid"].get<int>();

    // �洢������Ϣ
    _fridneModel.insert(userId, friendId);
}

// ����Ⱥ��ҵ�� Allgroup
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    // ����Ⱥ���û�id ����Ⱥ�� ����Ⱥ������
    int userid = js["id"].get<int>();
    string groupname = js["groupname"];
    string groupdesc = js["groupdesc"];

    // ������idΪ-1��Ϊ�����Զ�����������
    Group group(-1, groupname, groupdesc);
    if(_groupModel.createGroup(group)){
        // �洢Ⱥ�鴴���˵���Ϣ
        _groupModel.addGroup(userid, group.getId(), "creater");
    }
}

// ����Ⱥ��ҵ�� �û�id��Ⱥ��id
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    _groupModel.addGroup(userid, groupid, "normal");
}


