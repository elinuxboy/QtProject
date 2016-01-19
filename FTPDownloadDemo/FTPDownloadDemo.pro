#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T20:50:59
#
#-------------------------------------------------

QT       += core gui

QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FTPDownloadDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    myqtapi.h

FORMS    += mainwindow.ui

RESOURCES += \
    res/res.qrc
