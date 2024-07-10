#include "usermodel.hpp"
#include "mydb.hpp"
#include <string>
#include <iostream>

using namespace std;

// User���� ���ݲ����� UserModel ������ṩ���Ƕ��� �������ݿ��ֶ�

// User�������ӷ���
bool UserModel::insert(User &user){
    // 1.��װsql���
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')"
            , user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    // 2.connect�������ݿ�
    MySQL mysql;
    if(mysql.connect()){ 
        if(mysql.update(sql)){
            // ��ȡ����ɹ����û� �������ɵ�����id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

User UserModel::query(int id){
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);
    MySQL mysql;
    if(mysql.connect()){
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr){
            // fetch_row ��ѯ����
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr){
                User user;
                user.setId(atoi(row[0])); // row��������ַ��� atoi ��stringת��Ϊint
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);

                mysql_free_result(res); // �ͷ�ָ����Դ �����ڴ�й¶
                return user;
            }
        }
    }
    return User(); // Ĭ�Ϲ��� ����ֵidΪ-1
}

bool UserModel::updateState(User &user){
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d"
            , user.getState().c_str(), user.getId());

    MySQL mysql;
    if(mysql.connect()){
        if(mysql.update(sql)){
            return true;
        }
    }
    return false;
}

// �����û�״̬��Ϣ
void UserModel::resetState(){
    char sql[1024] = {0};
    string str = "offline";
    sprintf(sql, "update user set state = '%s' where state = 'online'", str.c_str());
    MySQL mysql;
    if(mysql.connect()){
        mysql.update(sql);
    }
}