#include "configreader.h"

ConfigReader::ConfigReader()
{
}
unordered_map<string, string> ConfigReader::readServerConfig(){
    ifstream in(SERVER_CONFIG_PATH, ios::in);
    unordered_map<string, string> configMap;  // 创建一个无序的map 装置map
    if(in.is_open()){  // 如果in打开了
        string line;  // 定义一个字符串类型
        while(getline(in, line)){
            int index = line.find('=');  // 寻找这个字符串里面的等于号
            configMap[line.substr(0, index)] = line.substr(index+1);  // configMap里面放置等号后面的字符串
        }
    }
    return configMap;  // 返回configMap
}
string ConfigReader::SERVER_CONFIG_PATH = "./config/server.conf";  // 指定服务器配置的路径
