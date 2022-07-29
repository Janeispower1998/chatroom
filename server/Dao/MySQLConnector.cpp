//
// Created by cswen on 2020/8/3.
//

#include "MySQLConnector.h"

void MySQLConnector::init() {
    conn = mysql_init(NULL);  // 初始化
    conn = mysql_real_connect(conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0);  // 尝试与MySQL服务器进行连接
    if (!conn) {  // 如果没连接上
        cout << "mysql_real_connect fail" << endl;
        exit(-1);  // 退出当前程序 将-1返回给主调程序
    }
    mysql_query(conn, "set names utf8");  // mysql_query是一条查询语句 "set names utf8"用于解决乱码问题 如果连接上了，就把conn转为UTF-8字符集
}

MySQLConnector *MySQLConnector::getMySQLConnector() {  // 获取MySQL连接器
    if (connector == nullptr) {  // 如果connector为空的话
        connector = new MySQLConnector;  // 则新建一个MySQL连接器的指针
        connector->init();  // 并初始化
    }
    return connector;  // 如果不是空的话，直接返回这个指针
}

pair<int, string> MySQLConnector::queryUser(const string& account, const string& password) {  // 这个函数用来查询MySQL数据库里面的账户和密码（用于登录的模块）
    string querySql =  // 下面这个就是MySQL语法的形式 并且把它转换成了字符串的形式
            "select account, username from user where account = " + account + " and password = " + "\"" + password +
            "\"";
    pair<int, string> user;  // 定义一个对对象user
    int res = mysql_query(conn, querySql.data());  // mysql_query是MySQL查询函数 conn连接器告诉他去查询querySql.data()这个数据库（也就是上面那个字符串）
                                                   // 查询成功则返回0 查询失败则返回非0
    if (!res) {
        MYSQL_RES *result = mysql_store_result(conn);  // 调用mysql_store_result查看结果集
        if (result && mysql_num_rows(result)) {  // 查看第一行有没有数据
            MYSQL_ROW row = mysql_fetch_row(result);  // 取出这个数据
            user.first = atoi(row[0]);  // 把account取出来转为int 放入user的第一位
            user.second = row[1];  // 把password取出来放在user第二位
        }
        mysql_free_result(result);  // 释放掉刚刚申请的资源
    }
    return user;  // 返回user就行
}

bool MySQLConnector::queryUser(const string& account) {  // 用于注册账户的时候 这个账户是否存在 如果已存在这个账户的话就不能再注册这个账户了
    string querySql = "select account from user where account = " + account;
    int res = mysql_query(conn, querySql.data());
    bool flag = false;
    if (!res) {
        MYSQL_RES *result = mysql_store_result(conn);
        if (result && mysql_num_rows(result)) {
            flag = true;
        }
        mysql_free_result(result);
    }
    return flag;
}

bool MySQLConnector::insertUser(const string& account, const string& username, const string& password) {  // 如果账户不存在的话 就添加账户
    string insertSql = "insert into `user` values(" +
                       account + ","
                                 "\"" + username + "\","
                       + "\"" + password + "\","
                       + "\"" + MyTime::getCurrentFormatTimeStr() + "\")";
    int res = mysql_query(conn, insertSql.data());
    return res == 0;
}

MySQLConnector::~MySQLConnector() {  // 析构掉
    mysql_close(conn);
    delete connector;
}
MySQLConnector *MySQLConnector::connector = nullptr;  // static的初始化
