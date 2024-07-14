#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

// 群组用户，多了一个role在群组内的角色信息，从User类继承
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