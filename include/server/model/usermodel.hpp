#ifndef USERMODEL_HPP
#define USERMODEL_HPP

#include "user.hpp"

// User��� ���ݲ�����
class UserModel{
public:
    // User������ӷ���
    bool insert(User &user);
    // �����û�id��ѯ�û���Ϣ
    User query(int id);
    // �����û�״̬��Ϣ
    bool updateState(User &user);
    // �����û�״̬��Ϣ
    void resetState();
private:
    
};




#endif