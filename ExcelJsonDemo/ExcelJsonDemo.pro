#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T10:20:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExcelJsonDemo
TEMPLATE = app
# 2015-12-25说明：该类使用COM操作excel，需要在.pro中加入CONFIG  += qaxcontainer
CONFIG  += qaxcontainer

SOURCES += main.cpp\
        mainwindow.cpp \
    lib/json/json_reader.cpp \
    lib/json/json_value.cpp \
    lib/json/json_valueiterator.inl \
    lib/json/json_writer.cpp \
    lib/excelengine/excelengine.cpp

HEADERS  += mainwindow.h \
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
    lib/excelengine/excelengine.h

FORMS    += mainwindow.ui

DISTFILES += \
    ico.rc

RESOURCES += \
    res.qrc
# 2015-12-29窗口图标文件
RC_FILE += ico.rc
