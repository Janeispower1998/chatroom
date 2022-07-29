//
// Created by cswen on 2020/8/3.
//
#pragma once
#include<iostream>
#include<string>
#include<unistd.h>
#include<vector>
#include"../ProtocolHead/protocolmsg.h"
#include<fstream>
#include<arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include"../Util/MyTime.h"
#include"../ProtocolHead/HeadData.h"
#include "../ProtocolHead/DataEncoder.h"

#define IMAGE_PATH "./image/"
#define TCP_BUFSIZ 8192  // TCP每次能传输的最大字节数为8192
using namespace std;

class DataProcesser {
    //协议号(1B)-账号(2B)-数据类型(1B)-数据长度(4B)-数据
private:
    char buffer[TCP_BUFSIZ];

    int checkSocketConnected(int sock);  // 每次收发之前都要检查一下TCP状态

public:
    DataProcesser();

    string readTextContent(int fd, unsigned int dataLength);

    string readImageContent(int fd, unsigned int dataLength);

    void writeText(int fd, unsigned int account, string text, unsigned int protocolId = SEND);

    void writeImage(int fd, unsigned int account, const string &imagePath);

    void writeMsg(int fd, unsigned int account, string text, unsigned int protocolId = SEND);

    void writeTextToAllUser(const vector<int> &fds, int account, const string &text, unsigned int protocolId = SEND);  // 群发text

    void writeImageToAllUser(const vector<int> &fds, int account, const string &imagePath);  // 群发image

    int getFileLength(const string &fileName);

};
