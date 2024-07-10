#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace muduo;

// 静态方法在类外实现 不用写static
ChatService* ChatService::instance()
{
    // 单例对象 线程安全
    static ChatService service;
    return &service;
}

// 注册消息以及对应的Handler回调
ChatService::ChatService()
{
    // 登陆
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    // 注册
    _msgHandlerMap.insert({REGISTE_MSG, std::bind(&ChatService::registe, this, _1, _2, _3)});
    // 一对一对话
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    // 添加好友列表
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    // 创建群组
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    // _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    // _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
}


// 获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgId)
{
    // 记录错误日志，msgId没有对应的事件处理器
    auto it = _msgHandlerMap.find(msgId); // 不能用[] []会引起新添加一个
    if(it == _msgHandlerMap.end())
    { 
        // muduo日志 #include <muduo/base/Logging.h>
        // 返回一个默认的处理器，空操作
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp){
            LOG_ERROR << "msgId: " << msgId << " can not find handler! ";
        };
    }
    else
    {
        return _msgHandlerMap[msgId];
    }
}

// 处理登陆业务   ORM  object relation map  对象关系映射  业务层操作的都是对象   业务模块  数据模块  解耦
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    int id = js["id"].get<int>(); // 当id作为主键时，进行的操作
    string password = js["password"];

    User user = _userModel.query(id);
    if(user.getId() == id && user.getPwd() == password){
        if(user.getState() == "online"){
            // 该用户已登陆 不允许重复登陆
            json response;
            response["msgId"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "user are inlined please reinsert";
            conn->send(response.dump()); // 序列化发送
        }
        else{
            // 登陆成功, 记录用户连接信息(多线程访问 要注意线程安全问题)
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({user.getId(), conn});
            }

             // 更新用户登陆状态 state:: offline->online
            user.setState("online"); 
            _userModel.updateState(user); // _userModel处理数据库

            // 登陆成功返回信息
            json response;
            response["msgId"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            
            // 查询该用户是否有离线消息 有则接收 离线消息保存在数据库中
            vector<string > vec = _offlineMsgModel.query(id);
            if(!vec.empty()){
                response["offlinemsg"] = vec;
                // 记录离线消息到json后 在数据库中删除离线消息
                _offlineMsgModel.drop(id);
            }

            // 查询用户的好友信息 并返回给客户端 js.dump
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

            conn->send(response.dump()); // 序列化发送
        }
    }else{
        // 用户不存在登陆失败，用户存在但密码错误，登陆失败
        json response;
        response["msgId"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "username Or password error";
        conn->send(response.dump()); // 序列化发送
    }
}

// 处理注册业务 反序列化json文件中的 name password
void ChatService::registe(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    string name = js["name"];
    string pwd = js["password"];

    // User对象创建  业务中操作的都是数据对象 不涉及数据库字段
    User user;
    user.setName(name);
    user.setPwd(pwd);
    // 数据库更新
    bool state = _userModel.insert(user);
    if(state){
        // 注册成功 响应 json
        json response;
        response["msgId"] = REGISTE_MSG_ACK; 
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump()); // json序列化发送
    }else{
        // 注册失败
        json response;
        response["msgId"] = REGISTE_MSG_ACK;
        response["errno"] = 1;
        // response["errmsg"] = REGISTE_MSG_FAILED;
        response["id"] = user.getId();
        conn->send(response.dump()); // json序列化发送
    }
}

// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr &conn)
{
    User user;
    // 处理异常退出之前，保证_userConnMap的线程安全问题 同时处理异常与login时的map
    {
        lock_guard<mutex> lock(_connMutex);
        for(auto it=_userConnMap.begin(); it!=_userConnMap.end(); ++it)
        {
            if(it->second == conn) // 查询到conn的智能指针
            {
                // 从unordered_map<int, conn>删除连接对
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1){  // 若为-1 则该id的用户不存在
        // 更新用户状态信息
        user.setState("offline");
        _userModel.updateState(user); // 数据库修改_userModel
    }
}

// 服务器异常，业务重置方法
void ChatService::reset(){
    // 数据库操作类usermodel把online状态的用户，设置成offline
    _userModel.resetState();
}

// 一对一聊天业务
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int toId = js["toid"].get<int>();

    {
        // 线程安全_userConnMap 连接信息表
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toId);
        if(it != _userConnMap.end()){
            // toId 在线 转发消息  it->second指向 toId的Conn
            // server主动推送消息给 toId用户
            it->second->send(js.dump());
            return;
        }
    }
    // toId 不在线 存储离线消息   离线消息表：offlineMsgModel  将离线消息存储到数据库中
    _offlineMsgModel.insert(toId, js.dump());
}

// 群组聊天业务
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    // 获取群聊转发对象列表
    vector<int> useridVec = _groupModel.queryGroupUsers(userid, groupid);
    // connmap表的线程安全
    lock_guard<mutex> lock(_connMutex);
    for(int id : useridVec){
        auto it =  _userConnMap.find(id);
        if(it != _userConnMap.end()){
            // 若该用户在线则直接转发消息
            it->second->send(js.dump());
        }else{
            // 若该用户不在线 则存储离线消息
            _offlineMsgModel.insert(id, js.dump());
        }
    }
}


// 添加好友业务
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userId = js["userid"].get<int>();
    int friendId = js["friendid"].get<int>();

    // 存储好友信息
    _fridneModel.insert(userId, friendId);
}

// 创建群组业务 Allgroup
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    // 创建群的用户id 创建群名 创建群的描述
    int userid = js["id"].get<int>();
    string groupname = js["groupname"];
    string groupdesc = js["groupdesc"];

    // 这里组id为-1，为的是自动增长的主键
    Group group(-1, groupname, groupdesc);
    if(_groupModel.createGroup(group)){
        // 存储群组创建人的信息
        _groupModel.addGroup(userid, group.getId(), "creater");
    }
}

// 加入群组业务 用户id，群组id
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time){
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    _groupModel.addGroup(userid, groupid, "normal");
}


