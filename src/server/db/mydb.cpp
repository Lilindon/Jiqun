#include "mydb.hpp"
#include <muduo/base/Logging.h>

static string server = "127.0.0.1";
static string user = "deku";
static string password = "123456";
static string dname = "chat";  // database name

// 初始化数据库连接 创建mysql连接的内存空间
MySQL::MySQL(){
    _conn = mysql_init(nullptr);
}

// 释放数据库连接资源
MySQL::~MySQL(){
    if(_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool  MySQL::connect(){
    /*  MYSQL host:ip地址 user:用户名 passwd:密码 db:数据库名 port:端口号  mysql默认为3306
        mysql_real_connect(MYSQL *mysql, const char *host,
                        const char *user, const char *passwd,
                        const char *db, unsigned int port,
                        const char *unix_socket,
                        unsigned long clientflag)*/
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(), password.c_str(), 
                                    dname.c_str(), 3306, nullptr, 0);
    if(p !=  nullptr){
        // c/c++ MySQL默认为ascll 不显示中文 set names gbk  让中文显示
        mysql_query(_conn, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }else{
        LOG_INFO << "connect mysql failed!";
    }
    return p;
}

// 更新操作 update 
bool MySQL::update(string sql){
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "更新失败!";
        return false;
    }
    return true;
}

// 查询操作 select
MYSQL_RES* MySQL::query(string sql){
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(_conn);
}

MYSQL* MySQL::getConnection(){
    return _conn;
}