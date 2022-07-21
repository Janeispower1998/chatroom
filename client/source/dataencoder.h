#pragma once
#ifndef ENCODEPACKET_H
#define ENCODEPACKET_H
#include"protocolmsg.h"
#include <string>
#include<QString>
using namespace std;
class DataEncoder
{
private:
    char head[BASE_BUFFER_SIZE];  // 定义一个字符串 缓存大小
    char * hp;  // 定义一个字符串指针
    void encodeElement(unsigned int data, unsigned int len);  // 定义一个编码元素的函数 它包括数据和长度两个参数

public:
    DataEncoder();
    string encode(unsigned int protocolId, unsigned int account, unsigned int dataType, unsigned int dataLength);
    // 定义一个字符串 它包含这些参数：协议ID 账号 数据类型 数据长度
};


#endif // ENCODEPACKET_H
