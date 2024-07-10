#include "offlinemessagemodel.hpp"
#include "mydb.hpp"

// �洢�û���������Ϣ
void OfflineMsgModel::insert(int userId, string msg){
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage(userid, message) values('%d', '%s')"
                , userId, msg.c_str());

    MySQL mysql;
    // .connect()�������ݿ� ����ֵΪbool
    if(mysql.connect()){
        mysql.update(sql);
    }
}
// ɾ���û���������Ϣ
void OfflineMsgModel::drop(int userId){
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid = '%d'",userId);
    
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql);
    }
}
// ��ѯ�û���������Ϣ
vector<string> OfflineMsgModel::query(int userId){
    char sql[1024] = {0};
    sprintf(sql, "select message from offlinemessage where userid = '%d'", userId);
    vector<string> vec;
    MySQL mysql;
    if(mysql.connect()){
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr){
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr ){
                // ��userId�û�������������Ϣ����vec��
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}

