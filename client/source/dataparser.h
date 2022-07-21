#ifndef DATAPARSER_H
#define DATAPARSER_H
#include"protocolmsg.h"
class DataParser
{
    //协议号(1B)-账号(2B)-数据类型(1B)-数据长度(4B)-数据
private:

    char* bp;  // 定义一个字符串指针bp
    unsigned int protocolId;  // 定义一个整型 协议ID
    unsigned int account;  // 账号
    unsigned int dataType;  // 数据类型
    unsigned int dataLength;  // 数据长度

    unsigned int parseInt(int len);  // 解析整型

public:

    DataParser(char * buffer);

    bool baseParse();  

    unsigned int getProtocolId();  // 定义一个函数 获取协议ID

    unsigned int getAccount();  // 定义一个函数 获取账号

    unsigned int getDataType();  // 定义一个函数 获取数据类型

    unsigned int getDataLength();  // 定义一个函数 获取数据长度

    ~DataParser();
};

#endif // DATAPARSER_H
