#include "groupmodel.hpp"
#include "mydb.hpp"

// Create Group
bool GroupModel::createGroup(Group &group){
    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup(groupname, groupdesc) values('%s', '%s')"
        , group.getName().c_str(), group.getDesc().c_str());
    MySQL mysql;
    if(mysql.connect()){
        if(mysql.update(sql)){
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}
// Add Group
void GroupModel::addGroup(int userid, int groupid, string role){
    char sql[1024] = {0};
    sprintf(sql, "insert into groupuser values(%d, %d, '%s')",
        groupid, userid, role.c_str());
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql);
    }

}
// Show User's Group ���������û���Ⱥ����Ϣ��vector
vector<Group> GroupModel::queryGroups(int userid){
    /*
      ���ϲ�ѯ AllGroup��GroupUser����id = idʱ���  
      ��ѯgroupid groupname groupdesc �����ڵ����г�Ա
    */
    char sql[1024] = {0};
    sprintf(sql,"select a.groupid, a.groupname, a.groupdesc from allgroup a inner join\
                 groupuser b on a.groupid = b.groupid where b.userid = %d",userid);

    vector<Group> groupVec;
    MySQL mysql;
    if(mysql.connect()){
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr){
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                groupVec.push_back(group);
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}
// ����ָ��groupid��ѯȺ���û�id�б� Ⱥ��Ϣͨ��һ���˷���Ⱥ��������ԱȺ����Ϣ
vector<int> GroupModel::queryGroupUsers(int userid, int groupid){
    char sql[1024] = {0};
    // ������������Աת����Ϣ Ⱥ��
    sprintf(sql, "select userid from groupuser where groupid = id and userid != %d", groupid, userid);

    vector<int> idVec;
    MySQL mysql;
    if(mysql.connect()){
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr){
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }
    return idVec;
}