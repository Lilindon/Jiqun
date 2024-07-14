#include "mydb.hpp"
#include <muduo/base/Logging.h>

static string server = "127.0.0.1";
static string user = "deku";
static string password = "123456";
static string dname = "chat";  // database name

// ��ʼ�����ݿ����� ����mysql���ӵ��ڴ�ռ�
MySQL::MySQL(){
    _conn = mysql_init(nullptr);
}

// �ͷ����ݿ�������Դ
MySQL::~MySQL(){
    if(_conn != nullptr)
        mysql_close(_conn);
}

// �������ݿ�
bool  MySQL::connect(){
    /*  MYSQL host:ip��ַ user:�û��� passwd:���� db:���ݿ��� port:�˿ں�  mysqlĬ��Ϊ3306
        mysql_real_connect(MYSQL *mysql, const char *host,
                        const char *user, const char *passwd,
                        const char *db, unsigned int port,
                        const char *unix_socket,
                        unsigned long clientflag)*/
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(), password.c_str(), 
                                    dname.c_str(), 3306, nullptr, 0);
    if(p !=  nullptr){
        // c/c++ MySQLĬ��Ϊascll ����ʾ���� set names gbk  ��������ʾ
        mysql_query(_conn, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }else{
        LOG_INFO << "connect mysql failed!";
    }
    return p;
}

// ���²��� update 
bool MySQL::update(string sql){
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "����ʧ��!";
        return false;
    }
    return true;
}

// ��ѯ���� select
MYSQL_RES* MySQL::query(string sql){
    if(mysql_query(_conn, sql.c_str())){
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" << sql << "��ѯʧ��!";
        return nullptr;
    }
    return mysql_use_result(_conn);
}

MYSQL* MySQL::getConnection(){
    return _conn;
}