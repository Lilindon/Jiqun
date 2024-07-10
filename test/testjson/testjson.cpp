#include "json.hpp"

using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// json序列化示例
void func1(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "lisi";
    js["msg"] = "hello, what are u doing now";

    // 将js序列化后转为字符串
    string sendBuf = js.dump();
    cout << sendBuf.c_str() <<endl;
    cout << js << endl;
}

// json序列化示例2
void func2(){
    json js;
    // 数组
    js["id"] = {1,2,3,4,5};
    // key-value
    js["name"] = "zhang san";
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["Li si"] = "hello wc";
    // 上面两句等于下面一句
    js["msg"] = {{"zhang san", "hello world"}, {"Li si","hello wc"}};
    cout << js <<endl;
}

// json序列化示例3
void func3(){
    json js;
    // 序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // 序列化一个map
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "黄山"});
    m.insert({3, "黄山"});

    js["path"] = m;
    // json序列化 json字符串
    string sendBuf = js.dump();
    cout << sendBuf << endl;
}

// json反序列化
string func5(){
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhangsan";
    js["to"] = "lisi";
    js["msg"] = "hello, what are u doing now";

    // 将js序列化后转为字符串
    string sendBuf = js.dump();
    return sendBuf;
}

// json反序列化示例2
string func6(){
    json js;
    // 数组
    js["id"] = {1,2,3,4,5};
    // key-value
    js["name"] = "zhang san";
    // 添加对象
    js["msg"]["zhang san"] = "hello world";
    js["msg"]["Li si"] = "hello wc";
    // 上面两句等于下面一句
    js["msg"] = {{"zhang san", "hello world"}, {"Li si","hello wc"}};
    cout << js <<endl;
}



// json反序列化示例3
string func7(){
    json js;
    // 序列化一个vector容器
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(5);
    js["list"] = vec;

    // 序列化一个map
    map<int, string> m;
    m.insert({1, "黄山"});
    m.insert({2, "黄山"});
    m.insert({3, "黄山"});

    js["path"] = m;
    // json序列化 json字符串
    string sendBuf = js.dump();
    return sendBuf;
}

int main(){
    // 数据序列化 json::dump() 将json 序列化为 字符串
    func1();
    // func2();
    // func3();

    // 数据反序列化 json::parse(string) 将 字符串 反序列化为 json
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