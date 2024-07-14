#pragma once

#include <hiredis/hiredis.h>
#include <string>
#include <functional>
#include <thread>
using namespace std;
class Redis{
public:
    Redis();
    ~Redis();

    bool connect();
    // 发布
    bool publish(int channel, string message);
    // 订阅
    bool subscribe(int channel);
    // 取消订阅
    bool unsubscribe(int channel);
    // 独立线程中 接收订阅通道中的消息
    void observer_channel_message();
    // 初始化  上报通道消息的回调对象
    void init_notify_handler(function<void(int, string)> fn);

private:
    // publish 上下文（客户端存储与连接相关的所有信息）
    redisContext *_publish_context;
    // subscribe 上下文（客户端存储与连接相关的所有信息）
    redisContext *_subscribe_context;
    // 事件回调，收到订阅消息，给ChatServer回调 service层 上报
    function<void(int, string)> _notify_message_handler;
};