#include "client.h"

Client::Client()
{

}

QJsonObject Client::readServerMsg(){
    readHeadData();  // 读取头部信息
    tcpClient.read(buffer, dataLength);  // 读取缓存和数据长度
    QString serverMsg = QString::fromStdString(string(buffer, dataLength));  // 封装string对象
    QJsonObject serverInfoObj = QJsonDocument::fromJson(serverMsg.toUtf8()).object();   // 封装JSON对象
    return serverInfoObj;  // 返回JSON对象的信息
}

void Client:: readHeadData(){
    int size = tcpClient.read(buffer, BASE_BUFFER_SIZE);  // 读取缓存和基本缓存大小
    DataParser parser(buffer);  // 数据解析器解析buffer
    parser.baseParse();  // 解析
    protocolId = parser.getProtocolId();  // 解析协议ID
    account = parser.getAccount();  // 解析账户
    dataType = parser.getDataType();  // 解析数据类型
    dataLength = parser.getDataLength();  // 解析数据长度
}

void Client:: writeText(unsigned int account ,string text, unsigned int protocolId){  // 写文字
    DataEncoder encoder;  // 封装一个编码器
    qDebug() << "------------输入的字节数-------------" << text.length();
    string headStr = encoder.encode(protocolId, account, TEXT, text.length());  // 用字符串来表示协议ID，账号，文字和文字长度的编码
    tcpClient.write(headStr.data(), headStr.length());  // 写出以上字符串的数据和长度
    qDebug() << "------------头部大小-------------" << headStr.length();
    //text = headStr + text;
    if(text.length() != 0){  // 如果文字长度不为零
        tcpClient.write(text.data(), text.length());  // 那就写出数据和长度
    }
    qDebug() << "------------发送成功,总数据大小-------------" << text.length();
}

QTcpSocket * Client::getTcpClient(){
    return &tcpClient;  // 返回tcpClient的地址
}

void Client:: closeTcpSocket(){  // 关掉TCP socket
    tcpClient.disconnectFromHost();
    tcpClient.close();
}

Client::~Client(){

}
