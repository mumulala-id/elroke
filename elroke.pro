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
#        aerobutton.cpp \
        proxymodel.cpp \
        delay.cpp \
        keyboard.cpp \
        editdb.cpp \
        dbmanager.cpp \
        opening.cpp \
    clineedit.cpp \
    song.cpp \
    songitemwidget.cpp \
    ProgressCircle.cpp

HEADERS  += mainwindow.h \
        player.h \
        about.h \
        addtodatabase.h \
#        aerobutton.h \
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
    ProgressCircle.h

TRANSLATIONS = file/languages/elroke_id.ts

DISTFILES +=

RESOURCES += \
    file.qrc
