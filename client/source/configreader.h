#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include<iostream>
#include<fstream>
#include<unordered_map>
using namespace std;
class ConfigReader
{
private:
    static string SERVER_CONFIG_PATH;  // 字符串静态变量 服务器装置的路径 
public:
    ConfigReader();  // 封装这个类
    static unordered_map<string, string> readServerConfig();  // 封装一个读取服务器装置的函数
};

#endif // CONFIGREADER_H
