#-------------------------------------------------
#
# Project created by QtCreator 2017-03-18T07:58:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elroke
TEMPLATE = app

LIBS += -lvlc  -lvlccore

SOURCES += main.cpp\
        mainwindow.cpp \
        player.cpp \
        about.cpp \
        addtodatabase.cpp \
        aerobutton.cpp \
        proxymodel.cpp \
        #database.cpp \
        delay.cpp \
        keyboard.cpp \
    editdb.cpp \
    dbmanager.cpp \
    opening.cpp

HEADERS  += mainwindow.h \
        player.h \
        about.h \
        addtodatabase.h \
        aerobutton.h \
        proxymodel.h \
        delay.h \
        keyboard.h \
    editdb.h \
    dbmanager.h \
    opening.h
        #database.h \

DISTFILES +=

CONFIG += console

RESOURCES += \
    file.qrc
