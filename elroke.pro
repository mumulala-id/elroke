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
    keyboard.h \
    dbmanager.h \
    liststringfileparser.h \
    clineedit.h \
    song.h \
    songitemwidget.h\
    youtube_downloader.h \
    preferences.h \
    videowidget.h \
    spinnerprogress.h \
    scoring.h \
    editdatabse.h \
    themewidget.h \
    theme.h

SOURCES += main.cpp\
    mainwindow.cpp \
    player.cpp \
    about.cpp \
    addtodatabase.cpp \
    proxymodel.cpp \
    keyboard.cpp \
    dbmanager.cpp \
    clineedit.cpp \
    song.cpp \
    songitemwidget.cpp \
    youtube_downloader.cpp \
    preferences.cpp \
    videowidget.cpp \
    spinnerprogress.cpp \
    scoring.cpp \
    editdatabase.cpp \
    themewidget.cpp \
    theme.cpp


TRANSLATIONS = file/languages/elroke_id.ts

CONFIG += c++14
RESOURCES += \
    file.qrc
