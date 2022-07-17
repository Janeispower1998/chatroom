QT       += core gui  # 使用core和gui库，其实这个是qt的默认设置，可以省略。如果要使用其他的库，数据库应该加上QT +=sql
QT += network  # 为了使用QTCreator开发程序

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  # 如果QT主版本大于4(也就是说使用Qt5或者更高版本)，则需要添加widgets模块

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS  # 如果使用标记为被弃用的Qt功能，它就会发出警告

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    configreader.cpp \
    dataencoder.cpp \
    dataparser.cpp \
    loginform.cpp \
    main.cpp \
    mytime.cpp \
    readclient.cpp \
    readthread.cpp \
    registerform.cpp \
    user.cpp \
    widget.cpp \
    writeclient.cpp

HEADERS += \
    client.h \
    configreader.h \
    dataencoder.h \
    dataparser.h \
    loginform.h \
    mytime.h \
    protocolmsg.h \
    readclient.h \
    readthread.h \
    registerform.h \
    user.h \
    widget.h \
    writeclient.h

FORMS += \
    loginform.ui \
    registerform.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES += \
    config/server.conf
