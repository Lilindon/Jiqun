#ifndef MYDB_HPP
#define MYDB_HPP

#include <string>
#include <mysql/mysql.h>

using namespace std;

class MySQL{
public:
    // ��ʼ�����ݿ����� ����mysql���ӵ��ڴ�ռ�
    MySQL();
    // �ͷ����ݿ�������Դ
    ~MySQL();
    // �������ݿ�
    bool connect();
    // ���²��� update 
    bool update(string sql);
    // ��ѯ���� select
    MYSQL_RES* query(string sql);
    // ��ȡ����  �����ⲿ�޷�����˽�г�Ա���� �����public�������� ����
    MYSQL* getConnection();

private:
    // Mysql.h���õ���������
    MYSQL *_conn;
};


#endif