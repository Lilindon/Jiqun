#ifndef USER_HPP
#define USER_HPP

#include <string>
using namespace std;

// User表的ORM 
class User{
public:
    // 默认值
    User(int id = -1, string name = "", string password = "", string state = "offline"){
        this->id = id;
        this->name = name;
        this->password = password;
        this->state = state;
    }
    // 设置成员变量
    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPwd(string password) { this->password = password; }
    void setState(string state) { this->state = state; }
    // 查询成员变量 
    int getId() { return this->id; }
    string getName() { return this->name; }
    string getPwd() { return this->password; }
    string getState() { return this->state; }
    
private:
    int id;
    string name;
    string password;
    string state;
};

#endif