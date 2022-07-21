#include "dataencoder.h"
#include<QDebug>
DataEncoder::DataEncoder() {

}

string DataEncoder::encode(unsigned int protocolId, unsigned int account, unsigned int dataType, unsigned int dataLength) {
    hp = head;
    encodeElement(protocolId, PROTOCOL_ID_SIZE);  // 协议ID 协议ID大小
    encodeElement(account, ACCOUNT_SIZE);  // 账号 账号大小
    encodeElement(dataType, DATA_TYPE_SIZE);  // 数据类型 数据类型大小
    encodeElement(dataLength, DATA_SIZE);  // 数据长度 数据大小
    return string(head,sizeof(head));  // 返回 头和头大小 的字符串
}

void DataEncoder::encodeElement(unsigned int data, unsigned int len) {  // 对数据和长度做一个编码
    char* c = hp + len - 1;
    for (int i = len; i > 0; i--) {
        *c = (char)(data & 0xff);
        c--;
        data >>= 8;
    }
    hp = hp + len;
}
