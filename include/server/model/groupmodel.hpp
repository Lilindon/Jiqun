#ifndef GROUPUSERMODEL_H
#define GROUPUSERMODEL_H

#include "group.hpp"
#include <vector>
#include <string>
using namespace std;

// 维护群组信息的操作接口方法
class GroupModel{
public:
    // Create Group
    bool createGroup(Group &group);
    // Add Group
    void addGroup(int userid, int groupid, string role);
    // Show User's Group
    vector<Group> queryGroups(int userid);
    // 根据指定groupid查询群组用户id列表vector<int>， 群消息通过一个人发给群内其他成员群发消息
    vector<int> queryGroupUsers(int userid, int groupid);
};  


#endif