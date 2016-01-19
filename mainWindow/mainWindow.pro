#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T19:04:10
#
#-------------------------------------------------

QT       += core gui
#使用网络模块
QT  += network

QT += multimedia

#说明：程序中使用了OWC10，用到了ActiveQt，原来的做法是在.pro中加CONFIG += qaxcontainer，现在需要改成QT += axcontainer当然了，原来的CONFIG += qaxcontainer也将去除
QT  += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mainWindow
TEMPLATE = app
#说明：该类使用COM操作excel，需要在.pro中加入CONFIG  += qaxcontainer
#CONFIG  += qaxcontainer
#关闭警告
CONFIG += warn_off

SOURCES += main.cpp\
        mainwindow.cpp \
    logindlg.cpp \
    optiondlg.cpp \
    lib/excelengine/excelengine.cpp \
    lib/json/json_reader.cpp \
    lib/json/json_value.cpp \
    lib/json/json_valueiterator.inl \
    lib/json/json_writer.cpp \
    lib/qextserialport/qextserialport.cpp \
    lib/qftp/qftp.cpp \
    lib/qftp/qurlinfo.cpp

#添加操作系统判断
win32 { SOURCES += lib/qextserialport/qextserialport_win.cpp }
unix { SOURCES += lib/qextserialport/qextserialport_unix.cpp }

HEADERS  += mainwindow.h \
    logindlg.h \
    optiondlg.h \
    lib/myqtapi.h \
    lib/excelengine/excelengine.h \
    lib/json/assertions.h \
    lib/json/autolink.h \
    lib/json/config.h \
    lib/json/features.h \
    lib/json/forwards.h \
    lib/json/json.h \
    lib/json/json_tool.h \
    lib/json/reader.h \
    lib/json/value.h \
    lib/json/version.h \
    lib/json/writer.h \
    lib/qextserialport/qextserialport.h \
    lib/qextserialport/qextserialport_global.h \
    lib/qextserialport/qextserialport_p.h \
    iniConfig.h \
    lib/qftp/qftp.h \
    lib/qftp/qurlinfo.h

FORMS    += mainwindow.ui \
    logindlg.ui \
    optiondlg.ui

#窗口图标文件
RC_FILE += ico.rc

RESOURCES += \
    res/res.qrc

DISTFILES += \
    ico.rc

SUBDIRS += \
    lib/qftp/qftp.pro
