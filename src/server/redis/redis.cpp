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

// ����redis
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
    // �����߳��� �����ܵ��¼���������Ϣ����ҵ����ϱ�
    thread t([&](){
        observer_channel_message();
    });
    t.detach();

    cout << " connect redis successfully " << endl;
}

// ����
bool Redis::publish(int channel, string message){
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message);
    if(reply == nullptr){
        cerr << " publish command failed " << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

// ����
bool Redis::subscribe(int channel){

    
}
// ȡ������
bool Redis::unsubscribe(int channel){}
// �����߳��� ���ն���ͨ���е���Ϣ
void Redis::observer_channel_message(){}
// ��ʼ��  �ϱ�ͨ����Ϣ�Ļص�����
void Redis::init_notify_handler(function<void(int, string)> fn){}