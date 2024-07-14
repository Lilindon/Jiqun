#include "redis.hpp"
#include <iostream>
using namespace std;

Redis::Redis() 
    : _publish_context(nullptr), _subscribe_context(nullptr){
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
    // 单独线程中 阻塞监听管道事件，若有消息则向业务层上报
    // 不能让ChatServer这个线程订阅后就阻塞，使得可以让线程执行其他任务
    thread t([&](){
        observer_channel_message();
    });
    t.detach();

    cout << " connect redis successfully " << endl;
}

// 发布   一发布立即就响应  不会阻塞publish线程
bool Redis::publish(int channel, string message){
    // redisCommand 1.将发送的命令缓存在本地 = redisAppendCommand
    //              2.将命令发送到redisServer中再调用redisGetReply = redisBufferwrite
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message);
    if(reply == nullptr){
        cerr << " publish command failed " << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

// 订阅  订阅channel后一直阻塞线程 同步发布订阅的问题
bool Redis::subscribe(int channel){
    /*
        SUBSCRIBE 命令会造成线程阻塞等待通道里发生的消息，这里只做订阅通道，不接收通道消息
        通道消息接收 专门在observer_channel_message函数中的 独立线程中运行
        只负责发命令，不阻塞接收redis server响应消息
        1个Chatserver四个线程 一个IO线程加三个worker线程
    */
    if(REDIS_ERR == redisAppendCommand(this->_subscribe_context, "SUBSCRIBE %d", channel)){
        cerr << " subscribe command failed " << endl;
    }
    // redisBufferWrite循环发送缓冲区，直到缓冲区数据发完 done=1
    int done = 0;
    while(!done){
        if(REDIS_ERR == redisBufferWrite(this->_subscribe_context, &done)){
            cerr << " subscribe command failed " << endl;
            return false;
        }
    }

    return true;
}

// 取消订阅
bool Redis::unsubscribe(int channel){
    if(REDIS_ERR == redisAppendCommand(this->_subscribe_context, "UNSUBSCRIBE %d", channel)){
        cerr << " unsubscribe command failed " << endl;
        return false;
    }
    // redisBufferWrite循环发送缓冲区，直到缓冲区数据发完 done=1
    int done = 0;
    while(!done){
        if(REDIS_ERR == redisBufferWrite(this->_subscribe_context, &done)){
            cerr << " unsubscribe command failed " << endl;
            return false;
        }
    }
    return true;
}

// 独立线程中 专门接收订阅通道中的消息
void Redis::observer_channel_message(){
    // 接收到通道的消息 返回三种信息
    // element
    // [0] 1) "message"
    // [1] 2) "13"
    // [2] 3) "hello world"
    /*
        connect   pub_context sub_context
                  pub上下文    sub上下文
        1.publish id message => pub_context
        2.subscribe id => sub_context （只发命令，不阻塞 用线程实现接收通道相应消息）
        3.thread  redisGetReply => sub_context
                  调用回调  给业务层上报 channel+message
    */
    redisReply *reply = nullptr;
    while(REDIS_OK == redisGetReply(this->_subscribe_context, (void **)&reply)){
        // 订阅收到通道的消息
        if(reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr){
            // 给业务层上报收到的消息 function<void(int, string)>   通道号 和 通道上发送的消息
            _notify_message_handler(atoi(reply->element[2]->str), reply->element[3]->str);
        }   
        freeReplyObject(reply);
    }
    cerr << " >>> observer_channel_message quit <<< " << endl;
}

// 初始化  上报通道消息的回调对象
void Redis::init_notify_handler(function<void(int, string)> fn){
    this->_notify_message_handler = fn;
}