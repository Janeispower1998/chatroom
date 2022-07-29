//
// Created by cswen on 2020/8/3.
//
#pragma once
#include<json/json.h>
#include<string>
#include"../Util/MyTime.h"
#include<unordered_map>
#include <utility>

using namespace std;

class Online {  // 定义用户的结构体
private:
    struct user {
        int account;
        string username;
        string loginTime;
    };
    unordered_map<int, int> writeFdToReadFd;  // 读文件操作符
    unordered_map<int, int> accountToFd;  // 写文件操作符
    unordered_map<int, user> userMap;  // 代码层面记录用户
public:
    string getOnlineListStr();  // 获取在线人数列表

    bool appendUser(int account, string username);  // 添加用户

    bool removeUser(int account);  // 移除用户

    bool appendUser(pair<int, string> &user);  // 添加用户的另一个重载函数

    int getReadFd(int writeFd);  

    vector<int> getAllReadFd();  // 获取所有的读描述符

    bool appendWriteFd(int account, int fd);  // 添加写描述符

    bool appendReadFd(int account, int fd);  // 添加读描述符

    string getUserJsonStr(int account);  // 获取用户的JSON形式

    string getUserName(int account);  // 获取用户的名字

    bool isLogin(int account);  // 判断是否登录

};
