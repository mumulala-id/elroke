
/*
    ElRoke
    Copyright (C) Muhammad Mukharom

    This file is part of ElRoke.
    ElRoke is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ElRoke is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with ElRoke.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QTranslator>
#include <QSplashScreen>
#include <QBitmap>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    // turn on the DPI support**
    QApplication a(argc, argv);
    QApplication::setStyle("plastique");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setOrganizationName("mumulala");
    QApplication::setOrganizationDomain("");
    QApplication::setApplicationName("ElRoke");
    QApplication::setApplicationVersion("0.0.1");
    QPixmap pic(":/usr/share/elroke/file/background/splash.png");
    QSplashScreen splash(pic);

   splash.show();
   a.processEvents();
   a.thread()->sleep(5);

    QTranslator t;
    t.load(":/usr/share/elroke/file/languages/elroke_id.qm");
    a.installTranslator(&t);

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"SQLITE is not installed";

    //create  database connection
    QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");

    mainWindow w;
    w.showFullScreen();
    splash.finish(&w);

    return a.exec();
}
