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
    // �����߳��� ���������ܵ��¼���������Ϣ����ҵ����ϱ�
    // ������ChatServer����̶߳��ĺ��������ʹ�ÿ������߳�ִ����������
    thread t([&](){
        observer_channel_message();
    });
    t.detach();

    cout << " connect redis successfully " << endl;
}

// ����   һ������������Ӧ  ��������publish�߳�
bool Redis::publish(int channel, string message){
    // redisCommand 1.�����͵�������ڱ��� = redisAppendCommand
    //              2.������͵�redisServer���ٵ���redisGetReply = redisBufferwrite
    redisReply *reply = (redisReply *)redisCommand(_publish_context, "PUBLISH %d %s", channel, message);
    if(reply == nullptr){
        cerr << " publish command failed " << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

// ����  ����channel��һֱ�����߳� ͬ���������ĵ�����
bool Redis::subscribe(int channel){
    /*
        SUBSCRIBE ���������߳������ȴ�ͨ���﷢������Ϣ������ֻ������ͨ����������ͨ����Ϣ
        ͨ����Ϣ���� ר����observer_channel_message�����е� �����߳�������
        ֻ�����������������redis server��Ӧ��Ϣ
        1��Chatserver�ĸ��߳� һ��IO�̼߳�����worker�߳�
    */
    if(REDIS_ERR == redisAppendCommand(this->_subscribe_context, "SUBSCRIBE %d", channel)){
        cerr << " subscribe command failed " << endl;
    }
    // redisBufferWriteѭ�����ͻ�������ֱ�����������ݷ��� done=1
    int done = 0;
    while(!done){
        if(REDIS_ERR == redisBufferWrite(this->_subscribe_context, &done)){
            cerr << " subscribe command failed " << endl;
            return false;
        }
    }

    return true;
}

// ȡ������
bool Redis::unsubscribe(int channel){
    if(REDIS_ERR == redisAppendCommand(this->_subscribe_context, "UNSUBSCRIBE %d", channel)){
        cerr << " unsubscribe command failed " << endl;
        return false;
    }
    // redisBufferWriteѭ�����ͻ�������ֱ�����������ݷ��� done=1
    int done = 0;
    while(!done){
        if(REDIS_ERR == redisBufferWrite(this->_subscribe_context, &done)){
            cerr << " unsubscribe command failed " << endl;
            return false;
        }
    }
    return true;
}

// �����߳��� ר�Ž��ն���ͨ���е���Ϣ
void Redis::observer_channel_message(){
    // ���յ�ͨ������Ϣ ����������Ϣ
    // element
    // [0] 1) "message"
    // [1] 2) "13"
    // [2] 3) "hello world"
    /*
        connect   pub_context sub_context
                  pub������    sub������
        1.publish id message => pub_context
        2.subscribe id => sub_context ��ֻ����������� ���߳�ʵ�ֽ���ͨ����Ӧ��Ϣ��
        3.thread  redisGetReply => sub_context
                  ���ûص�  ��ҵ����ϱ� channel+message
    */
    redisReply *reply = nullptr;
    while(REDIS_OK == redisGetReply(this->_subscribe_context, (void **)&reply)){
        // �����յ�ͨ������Ϣ
        if(reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr){
            // ��ҵ����ϱ��յ�����Ϣ function<void(int, string)>   ͨ���� �� ͨ���Ϸ��͵���Ϣ
            _notify_message_handler(atoi(reply->element[2]->str), reply->element[3]->str);
        }   
        freeReplyObject(reply);
    }
    cerr << " >>> observer_channel_message quit <<< " << endl;
}

// ��ʼ��  �ϱ�ͨ����Ϣ�Ļص�����
void Redis::init_notify_handler(function<void(int, string)> fn){
    this->_notify_message_handler = fn;
}