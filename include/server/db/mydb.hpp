#ifndef MYDB_HPP
#define MYDB_HPP

#include <string>
#include <mysql/mysql.h>

using namespace std;

class MySQL{
public:
    // 初始化数据库连接 创建mysql连接的内存空间
    MySQL();
    // 释放数据库连接资源
    ~MySQL();
    // 连接数据库
    bool connect();
    // 更新操作 update 
    bool update(string sql);
    // 查询操作 select
    MYSQL_RES* query(string sql);
    // 获取连接  对象外部无法访问私有成员变量 因此在public创建函数 返回
    MYSQL* getConnection();

private:
    // Mysql.h内置的数据类型
    MYSQL *_conn;
};


#endif