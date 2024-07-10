#ifndef PUBLIC_HPP
#define PUBLIC_HPP

/*
server��client�Ĺ����ļ�
*/
enum EnMsgType{
    LOGIN_MSG = 1,      // ��½
    LOGIN_MSG_ACK = 2,  // ��½��Ӧ
    REGISTE_MSG = 3,    // ע��
    REGISTE_MSG_ACK = 4,// ע����Ӧ
    ONE_CHAT_MSG = 5,   // ������Ϣ
    ADD_FRIEND_MSG = 6, // ��Ӻ�����Ϣ
    CREATE_GROUP_MSG = 7,// ����Ⱥ��
    ADD_GROUP_MSG = 8,  // ���Ⱥ��
    GROUP_CHAT_MSG = 9, // Ⱥ������
    LOGINOUT_MSG = 10,  // �ǳ�
};

struct ErrMsg{
    string REGISTE_MSG_FAILED = "register failed";
};

#endif