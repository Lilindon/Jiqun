#ifndef GROUP_HPP
#define GROUP_HPP

#include "groupuser.hpp"
#include <string>
#include <vector>
using namespace std;

class Group{
public:
    Group(int id = -1, string name = "", string desc = ""){
        this->_id = id;
        this->_name = name;
        this->_desc = desc;
    }

    void setId(int id){
        this->_id = id;
    }
    string setName(string name){
        this->_name = name;
    }
    string setDesc(string desc){
        this->_desc = desc;
    }

    int getId() { return this->_id; }
    string getName() { return this->_name; }
    string getDesc() { return this->_desc; }
    vector<GroupUser> &getUsers() { return this->_users; }

private:
    int _id;
    string _name;
    string _desc;
    vector<GroupUser> _users;
};

#endif