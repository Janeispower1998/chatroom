#ifndef CLIENT_H
#define CLIENT_H

#include<QTcpSocket>
#include<QtAlgorithms>
#include<string>
#include<fstream>
#include <QMessageBox>
#include<QDebug>
#include <QTextCodec>
#include<QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include"dataparser.h"
#include"dataencoder.h"
#include"configreader.h"
#define IMAGE_PATH "./image/"
#define TCP_BUFSIZ 8192  // TCP缓存大小是8192
using namespace std;

class Client
{
public:
    QString hostName;  // host
    quint16 port;  // port 16为无符号整型
    QTcpSocket tcpClient;  // socket
    char buffer[TCP_BUFSIZ];  // 缓存
    unsigned int protocolId;  // 协议ID
    unsigned int account;  // 账号
    unsigned int dataType;  // 数据类型
    unsigned int dataLength;  // 数据长度
    const string ACK_PACKET = DataEncoder().encode(ACK,0,0,0);

public:
    Client();

    void readHeadData();  // 定义一个读取头信息的函数

    QJsonObject readServerMsg();  // 封装一个JSON对象

    void writeText(unsigned int account ,string text, unsigned int protocolId = SEND);  // 封装一个写文字的函数

    void closeTcpSocket();  // 封装一个关掉TCP socket的函数

    QTcpSocket * getTcpClient();  // 

    ~Client();  // 析构
};

#endif // CLIENT_H
