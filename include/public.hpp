#ifndef PUBLIC_HPP
#define PUBLIC_HPP

/*
server与client的公共文件
*/
enum EnMsgType{
    LOGIN_MSG = 1,      // 登陆
    LOGIN_MSG_ACK = 2,  // 登陆响应
    REGISTE_MSG = 3,    // 注册
    REGISTE_MSG_ACK = 4,// 注册响应
    ONE_CHAT_MSG = 5,   // 聊天消息
    ADD_FRIEND_MSG = 6, // 添加好友消息
    CREATE_GROUP_MSG = 7,// 创建群组
    ADD_GROUP_MSG = 8,  // 添加群组
    GROUP_CHAT_MSG = 9, // 群组聊天
    LOGINOUT_MSG = 10,  // 登出
};

struct ErrMsg{
    string REGISTE_MSG_FAILED = "register failed";
};

#endif