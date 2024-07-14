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
    // ����
    bool publish(int channel, string message);
    // ����
    bool subscribe(int channel);
    // ȡ������
    bool unsubscribe(int channel);
    // �����߳��� ���ն���ͨ���е���Ϣ
    void observer_channel_message();
    // ��ʼ��  �ϱ�ͨ����Ϣ�Ļص�����
    void init_notify_handler(function<void(int, string)> fn);

private:
    // publish �����ģ��ͻ��˴洢��������ص�������Ϣ��
    redisContext *_publish_context;
    // subscribe �����ģ��ͻ��˴洢��������ص�������Ϣ��
    redisContext *_subscribe_context;
    // �¼��ص����յ�������Ϣ����ChatServer�ص� service�� �ϱ�
    function<void(int, string)> _notify_message_handler;
};