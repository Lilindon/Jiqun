#include "redis.hpp"
#include <iostream>
using namespace std;

Redis::Redis() : _publish_context(nullptr), _subscribe_context(nullptr){
}


Redis::~Redis(){
    if(_publish_context != nullptr){
        redisFree(_publish_context);
    }
    if(_subscribe_context != nullptr){
        redisFree(_subscribe_context);
    }
}

// 连接redis
bool Redis::connect(){
    _publish_context = redisConnect("127.0.0.1",6379);
    if(_publish_context == nullptr){
        cerr << " connect redis failed " << endl;
        return false;
    } 
    _subscribe_context = redisConnect("127.0.0.1",6379);
    if(_subscribe_context == nullptr){
        cerr << " connect redis failed " << endl;
        return false;
    }
    // 单独线程中 监听管道事件，若有消息则向业务层上报
    thread t([&](){
        observer_channel_message();
    });
    t.detach();

    cout << " connect redis successfully " << endl;
}

// 发布
bool Redis::publish(int channel, string message){
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message);
    if(reply == nullptr){
        cerr << " publish command failed " << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

// 订阅
bool Redis::subscribe(int channel){

    
}
// 取消订阅
bool Redis::unsubscribe(int channel){}
// 独立线程中 接收订阅通道中的消息
void Redis::observer_channel_message(){}
// 初始化  上报通道消息的回调对象
void Redis::init_notify_handler(function<void(int, string)> fn){}