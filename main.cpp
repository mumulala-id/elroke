
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
//#include <stdlib.h>

int main(int argc, char *argv[])
{
    // turn on the DPI support**

QApplication::setStyle("plastique");
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("mumulala");
   QCoreApplication::setOrganizationDomain("");
   QCoreApplication::setApplicationName("ElRoke");
   QCoreApplication::setApplicationVersion("3.4.2");
   QPixmap pic(":/usr/share/elroke/file/background/splash.png");
   QSplashScreen splash(pic);

   splash.show();
  a.processEvents();
   a.thread()->sleep(3);


    QTranslator t;
    t.load(":/usr/share/elroke/file/languages/elroke_id.qm");
    a.installTranslator(&t);

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"SQLITE is not installed";

    //create  database connection
   QSqlDatabase A=  QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase B = QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase C = QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");

    mainWindow w;
    w.showFullScreen();
    splash.finish(&w);


    return a.exec();
}
