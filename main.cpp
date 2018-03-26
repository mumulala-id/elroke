
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
#include <QSplashScreen>
#include <QBitmap>
#include <QDebug>
#include <QStyleFactory>
#include <QLocale>
#include <QTranslator>
#include <QSettings>
int main(int argc, char *argv[])
{
    // turn on the DPI support**
    QApplication a(argc, argv);
    QApplication::setStyle("plastique");
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setOrganizationName("mumulala");
    QApplication::setOrganizationDomain("");
    QApplication::setApplicationName("elroke");
    QApplication::setApplicationVersion("0.0.1");
    QPixmap pic(":/usr/share/elroke//icon/splash.png");
    QSplashScreen splash(pic);

   splash.show();
   a.processEvents();
   a.thread()->sleep(5);



    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"SQLITE is not installed";

    //create  database connection
    QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");

    QSettings setting("elroke", "elroke");
    setting.beginGroup("Preferences");
    QString language = setting.value("language").toString();
    setting.endGroup();

//    QString language = QLocale::languageToString(QLocale().system().language());
    qDebug()<<language;
    QTranslator t;
    if(language!="english"&&language!=NULL)
    {
        t.load(":/usr/share/elroke/languages/elroke_id.qm");
        a.installTranslator(&t);
     }

    mainWindow w;
    w.showFullScreen();
    splash.finish(&w);

    return a.exec();
}
