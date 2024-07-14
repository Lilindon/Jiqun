#ifndef CHATSERVICE_H  // 防止头文件重复包含
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

// c++11 语法 定义新的类型名称
// 处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 聊天服务器业务类
class ChatService
{
public:
    // 处理登陆业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void registe(const TcpConnectionPtr &conn, json &js, Timestamp time);
    
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgId);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);
    // 处理服务器异常退出，业务重置方法
    void reset();
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void handleRedisSubscribeMessage(int userid, string msg);
    // 获取单例对象的接口函数
    static ChatService* instance();

private:
    // 单例模式：1.构造函数 私有化 2.写一个惟一的实例 3.再暴露一个接口
    ChatService();

    // 存储消息id 和其业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;
    
    // 存储在线用户的通信连接 (访问多，读写，注意线程安全)
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 定义互斥锁 保证_userConnMap的线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;

    // 离线消息操作对象  
    OfflineMsgModel _offlineMsgModel;

    // 添加好友类对象
    FriendModel _fridneModel;

    // 组类对象
    GroupModel _groupModel;

    // Redis操作对象
    Redis _redis;
};

#endif