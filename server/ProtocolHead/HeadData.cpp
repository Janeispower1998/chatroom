//
// Created by cswen on 2020/8/3.
//

#include "HeadData.h"

bool HeadData::baseParse() {  // 解析数据
    this->protocolId = parseInt(PROTOCOL_ID_SIZE);
    this->account = parseInt(ACCOUNT_SIZE);
    this->dataType = parseInt(DATA_TYPE_SIZE);
    this->dataLength = parseInt(DATA_SIZE);
}

unsigned int HeadData::parseInt(int len) {
    unsigned int sum = 0;
    unsigned int i = 0;
    for (const char *end = bp + len - 1; bp <= end; end--) {
        sum = sum + (((unsigned int) ((unsigned char) (*end))) << i);
        i += 8;
    }
    bp = bp + len;
    return sum;
}

HeadData::HeadData(int fd) {  // fd即socket的意思
    read(fd, buffer, BASE_BUFFER_SIZE);  // 然后读socket的8位缓存
    bp = buffer;
    baseParse();  // 然后开始解析
}

HeadData::HeadData() {

}

bool HeadData::parse(const char *buffer) {
    bp = buffer;
    baseParse();
}

unsigned int HeadData::getProtocolId() {
    return this->protocolId;
}

unsigned int HeadData::getAccount() {
    return this->account;
}

unsigned int HeadData::getDataType() {
    return this->dataType;
}

unsigned int HeadData::getDataLength() {
    return this->dataLength;
}
