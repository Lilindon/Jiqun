#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

// Ⱥ���û�������һ��role��Ⱥ���ڵĽ�ɫ��Ϣ����User��̳�
class GroupUser : public User {
public:
    void setRole(string role){
        this->_role = role;
    }
    string getRole(){
        return this->_role; 
    }

private:
    string _role;
};

#endif