#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<arpa/inet.h>
#include<cstdlib>
#include<unistd.h>
#include<sys/epoll.h>
#include<json/json.h>
#include"ProtocolHead/HeadData.h"
#include"Service/DataProcesser.h"
#include "Service/UserService.h"
#include "Service/Online.h"
#include "config/server_config.h"

using namespace std;

int main() {
    int lfd = socket(AF_INET, SOCK_STREAM, 0);  // 申请socket过程
    sockaddr_in serverAddr{}, clientAddr{};  // 申请地址结构体
    int opt = 1;
    if (-1 == setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {  // setsockopt设置socket选项
        cout << "setsockopt fail" << endl;
        exit(-1);
    }//设置端口复用
    int epfd = epoll_create(MAX_CONNECTIONS);  // epoll_create创建一个epoll实例 MAX_CONNECTIONS参数可以忽略掉 大于0就行
    epoll_event ev{}, events[MAX_CONNECTIONS];  // epoll_event用于注册感兴趣事件和回传所发生的待处理事件 events表示感兴趣事件和被触发事件
    ev.data.fd = lfd;  // 传一个客户socket的句柄
    ev.events = EPOLLIN;  // EPOLLIN表示对应文件描述符可读
    if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev)) {  // epoll_ctl注册事件
        cout << "epoll_ctl fail" << endl;
        exit(-1);
    }
    serverAddr.sin_port = htons(PORT);  // socket将分配PORT这个端口
    serverAddr.sin_family = AF_INET;  // AF_INET表示互联网域名
    inet_pton(AF_INET, HOST, &serverAddr.sin_addr);  // inet_pton是IP地址转换函数
    if (-1 == bind(lfd, (sockaddr *) &serverAddr, sizeof(serverAddr))) {
        cout << "bind fail" << endl;
        exit(-1);
    }
    if (-1 == listen(lfd, MAX_CONNECTIONS)) {
        cout << "listen fail" << endl;
        exit(-1);
    }
    cout << "listening..." << endl;
    char ipAddress[BUFSIZ];
    UserService us;
    Online online;
    while (true) {
        int nready = epoll_wait(epfd, events, MAX_CONNECTIONS, -1);  // epoll_wait获取是否有注册事件发生
        if (nready < 0) {
            cout << "epoll_wait error" << endl;
            exit(-1);
        }
        cout << "收到" << nready << "个请求" << endl;
        for (int i = 0; i < nready; i++) {
            int fd = events[i].data.fd;
            if (fd == lfd) {  // 判断是否有客户端连接进来
                socklen_t len = sizeof(clientAddr);
                int cfd = accept(lfd, (sockaddr *) &clientAddr, &len);
                ev.data.fd = cfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
                inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress, sizeof(clientAddr));
                //设置超时read
                struct timeval timeout = {1, 0};
                setsockopt(cfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));
            } else if (events[i].events & EPOLLIN) {  // 如不是 就是有客户端断开或客户端通信
                HeadData hd(fd);
                unsigned int protocolId = hd.getProtocolId();
                unsigned int account = hd.getAccount();
                unsigned int dataType = hd.getDataType();
                unsigned int dataLength = hd.getDataLength();
                DataProcesser dp;
                switch (protocolId) {
                    case LOGIN: {
                        string loginMsg = dp.readTextContent(fd, dataLength);
                        Json::Reader jsonReader;
                        Json::Value msg;
                        jsonReader.parse(loginMsg, msg);
                        string account = msg["account"].asString();
                        string password = msg["password"].asString();
                        pair<int, string> user = us.checkLogin(account, password);
                        Json::Value loginResult;
                        //登录成功
                        if (user.first != 0) {
                            if (online.isLogin(user.first)) {
                                loginResult["status"] = LOGIN_EXIST;
                            } else {
                                online.appendUser(user);
                                online.appendWriteFd(user.first, fd);
                                loginResult["status"] = LOGIN_SUCCESS;
                                loginResult["username"] = user.second;
                            }
                        }
                        //失败
                        else {
                            loginResult["status"] = LOGIN_FAIL;
                        }
                        string loginResultStr = loginResult.toStyledString();
                        dp.writeMsg(fd, 0, loginResultStr);
                    }
                        break;
                    case REGISTER: {
                        string registerMsg = dp.readTextContent(fd, dataLength);
                        Json::Reader jsonReader;
                        Json::Value registerResult;
                        Json::Value msg;
                        jsonReader.parse(registerMsg, msg);
                        string account = msg["account"].asString();
                        string username = msg["username"].asString();
                        string password = msg["password"].asString();
                        if (us.isRegistered(account) || !us.registerUser(account, username, password)) {
                            registerResult["status"] = REGISTER_FAIL;
                        } else {
                            registerResult["status"] = REGISTER_SUCCESS;
                        }
                        dp.writeMsg(fd, 0, registerResult.toStyledString(), REGISTER);
                    }
                        break;

                    case SEND: {
                        string baseMsg = online.getUserName(account) + "(" + to_string(account) + ")说:";
                        if (dataType == TEXT) {
                            dp.writeTextToAllUser(online.getAllReadFd(), account, baseMsg);
                            string content = dp.readTextContent(fd, dataLength);
                            dp.writeTextToAllUser(online.getAllReadFd(), account, content);
                        } else if (dataType == IMAGE) {
                            string imagePath = dp.readImageContent(fd, dataLength);
                            if (dp.getFileLength(imagePath) == dataLength) {
                                dp.writeTextToAllUser(online.getAllReadFd(), account, baseMsg);
                                dp.writeImageToAllUser(online.getAllReadFd(), account, imagePath);
                            } else {
                                ev.data.fd = fd;
                                ev.events = EPOLLIN;
                                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                                close(fd);
                                close(online.getReadFd(fd));
                                string logoutMsg =
                                        online.getUserName(account) + "(" + to_string(account) + ")" + "离开了聊天室!";
                                online.removeUser(account);
                                vector<int> fds = online.getAllReadFd();
                                if (!fds.empty()) {
                                    dp.writeTextToAllUser(fds, account, logoutMsg, NOTICE);
                                    dp.writeTextToAllUser(fds, 0, online.getOnlineListStr(), ONLINELIST);
                                }
                            }
                        }
                    }
                        break;
                    case READ: {
                        ev.data.fd = fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                        online.appendReadFd(account, fd);
                        string loginMsg = online.getUserName(account) + "(" + to_string(account) + ")" + "走进了聊天室!";
                        dp.writeTextToAllUser(online.getAllReadFd(), account, loginMsg, NOTICE);
                        dp.writeTextToAllUser(online.getAllReadFd(), account, online.getOnlineListStr(), ONLINELIST);
                    }
                        break;
                    case LOGOUT: {
                        sleep(1);
                        ev.data.fd = fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                        close(fd);
                        close(online.getReadFd(fd));
                        string logoutMsg = online.getUserName(account) + "(" + to_string(account) + ")" + "离开了聊天室!";
                        online.removeUser(account);
                        vector<int> fds = online.getAllReadFd();
                        cout << "当前在线人数:" << fds.size() << endl;
                        if (!fds.empty()) {
                            dp.writeTextToAllUser(fds, account, logoutMsg, NOTICE);
                            dp.writeTextToAllUser(fds, 0, online.getOnlineListStr(), ONLINELIST);
                        }
                    }
                        break;
                    case CLOSE: {
                        sleep(1);
                        ev.data.fd = fd;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
                        close(fd);
                    }
                        break;
                }

            }
        }
    }

    close(lfd);
    return 0;
}
