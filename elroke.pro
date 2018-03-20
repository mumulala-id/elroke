#-------------------------------------------------
#
# Project created by QtCreator 2017-03-18T07:58:09
#
#-------------------------------------------------

QT       += core gui sql webkitwidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = elroke
TEMPLATE = app

LIBS += -lvlc  -lvlccore

HEADERS  += mainwindow.h \
    player.h \
    about.h \
    addtodatabase.h \
    proxymodel.h \
    delay.h \
    keyboard.h \
    editdb.h \
    dbmanager.h \
    opening.h \
    liststringfileparser.h \
    clineedit.h \
    song.h \
    songitemwidget.h\
    ProgressCircle.h \
    listwidget.h \
    youtube_downloader.h \
    preferences.h \
    videowidget.h


SOURCES += main.cpp\
    mainwindow.cpp \
    player.cpp \
    about.cpp \
    addtodatabase.cpp \
    proxymodel.cpp \
    delay.cpp \
    keyboard.cpp \
    editdb.cpp \
    dbmanager.cpp \
    opening.cpp \
    clineedit.cpp \
    song.cpp \
    songitemwidget.cpp \
    ProgressCircle.cpp \
    listwidget.cpp \
    youtube_downloader.cpp \
    preferences.cpp \
    videowidget.cpp

TRANSLATIONS = file/languages/elroke_id.ts

DISTFILES +=
CONFIG += c++14
RESOURCES += \
    file.qrc
