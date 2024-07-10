#include "json.hpp"

using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// json���л�ʾ��
void func1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "lisi";
    js["msg"] = "hello, what are u doing now";

    // ��js���л���תΪ�ַ���
    string sendBuf = js.dump();
    cout << sendBuf.c_str() <<endl;
    cout << js << endl;
}

// json���л�ʾ��2
void func2(){
    json js;
    // ����
    js["id"] = {1,2,3,4,5};
    // key-value
    js["name"] = "zhang san";
    // ��Ӷ���
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["Li si"] = "hello wc";
    // ���������������һ��
    js["msg"] = {{"zhang san", "hello world"}, {"Li si","hello wc"}};
    cout << js <<endl;
}

// json���л�ʾ��3
void func3(){
    json js;
    // ���л�һ��vector����
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // ���л�һ��map
    map<int, string> m;
    m.insert({1, "��ɽ"});
    m.insert({2, "��ɽ"});
    m.insert({3, "��ɽ"});

    js["path"] = m;
    // json���л� json�ַ���
    string sendBuf = js.dump();
    cout << sendBuf << endl;
}

// json�����л�
string func5(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "lisi";
    js["msg"] = "hello, what are u doing now";

    // ��js���л���תΪ�ַ���
    string sendBuf = js.dump();
    return sendBuf;
}

// json�����л�ʾ��2
string func6(){
    json js;
    // ����
    js["id"] = {1,2,3,4,5};
    // key-value
    js["name"] = "zhang san";
    // ��Ӷ���
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["Li si"] = "hello wc";
    // ���������������һ��
    js["msg"] = {{"zhang san", "hello world"}, {"Li si","hello wc"}};
    cout << js <<endl;
}



// json�����л�ʾ��3
string func7(){
    json js;
    // ���л�һ��vector����
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // ���л�һ��map
    map<int, string> m;
    m.insert({1, "��ɽ"});
    m.insert({2, "��ɽ"});
    m.insert({3, "��ɽ"});

    js["path"] = m;
    // json���л� json�ַ���
    string sendBuf = js.dump();
    return sendBuf;
}

int main(){
    // �������л� json::dump() ��json ���л�Ϊ �ַ���
    func1();
    // func2();
    // func3();

    // ���ݷ����л� json::parse(string) �� �ַ��� �����л�Ϊ json
    // string recvBuf = func5();
    // json buf = json::parse(recvBuf);
    // cout << buf << endl;
    // cout << buf["msg_type"] << endl;

    // string recvBuf2 = func6();
    // json buffer = json::parse(recvBuf2);
    // cout << buffer << endl;
    // cout << buffer["id"] << endl;
    // auto arr = buffer["id"];
    // cout << arr[2] << endl; 
    
    // string recvBuf3 = func7();
    // json bufffer = json::parse(recvBuf3);
    // map<int, string> mymap = bufffer["path"];
    // for(auto &p : mymap){
    //     cout << p.first << " "<< p.second << endl;
    // }

    return 0;
}