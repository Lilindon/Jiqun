#ifndef GROUPUSERMODEL_H
#define GROUPUSERMODEL_H

#include "group.hpp"
#include <vector>
#include <string>
using namespace std;

// ά��Ⱥ����Ϣ�Ĳ����ӿڷ���
class GroupModel{
public:
    // Create Group
    bool createGroup(Group &group);
    // Add Group
    void addGroup(int userid, int groupid, string role);
    // Show User's Group
    vector<Group> queryGroups(int userid);
    // ����ָ��groupid��ѯȺ���û�id�б�vector<int>�� Ⱥ��Ϣͨ��һ���˷���Ⱥ��������ԱȺ����Ϣ
    vector<int> queryGroupUsers(int userid, int groupid);
};  


#endif