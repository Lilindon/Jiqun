#include "offlinemessagemodel.hpp"
#include "mydb.hpp"

// 存储用户的离线消息
void OfflineMsgModel::insert(int userId, string msg){
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage(userid, message) values('%d', '%s')"
                , userId, msg.c_str());

    MySQL mysql;
    // .connect()连接数据库 返回值为bool
    if(mysql.connect()){
        mysql.update(sql);
    }
}
// 删除用户的离线消息
void OfflineMsgModel::drop(int userId){
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid = '%d'",userId);
    
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql);
    }
}
// 查询用户的离线消息
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
                // 将userId用户的所有离线消息放入vec中
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}

