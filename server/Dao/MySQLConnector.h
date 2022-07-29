//
// Created by cswen on 2020/8/3.
//

#include <iostream>
#include<string>
#include <mysql/mysql.h>
#include <unordered_map>
#include<json/json.h>
#include "../Util/MyTime.h"
#include "../config/mysql_config.h"
using namespace std;

// 声明一个连接器
class MySQLConnector {
private:  // 只能在这个类的内部用private里面的东西
    MYSQL *conn;  // MYSQL是<mysql/mysql.h>中的一个类，声明了这个类的一个指针conn
    static MySQLConnector *connector;  // 声明这个连接器自己的指针 它是静态的 就是不管生成多少个连接器对象 那些连接器对象都与这个是关联的
    void init();  // 初始化

public:  
    static MySQLConnector *getMySQLConnector();  // 找到连接器 服务器需要找到MySQL连接器的一个东西

    pair<int, string> queryUser(const string& account, const string& password);

    bool queryUser(const string& account);

    bool insertUser(const string& account, const string& username, const string& password);

    ~MySQLConnector();
};
