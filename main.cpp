
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
#include <QThread>
#include <QSettings>

//class  I : public QThread
//{
//public:
//    static void sleep (unsigned long secs){QThread::sleep(secs);}
//};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("plastique");
//    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setOrganizationName("mumulala");
    QApplication::setOrganizationDomain("");
    QApplication::setApplicationName("elroke");
    QApplication::setApplicationVersion("0.0.1");

    QPixmap pic(":/usr/share/elroke/icon/logo.png");
    QSplashScreen splash(pic);
     splash.show();
//    I::sleep(5);

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"SQLITE is not installed";

    //create  database connection
    QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");


    QSettings setting("elroke", "elroke");
    setting.beginGroup("Preferences");

    auto setDefault = [&setting](QString name, QVariant variant){

        if(!setting.contains(name))
            setting.setValue(name, variant);
    };
    setDefault("monthRange",3);
    setDefault("fontSize",12);
    setDefault("background","/usr/share/elroke/background/redlove.jpeg");
    setDefault("fontName","Roboto");
    setDefault("favGroup",QStringList()<<"POP"<<"ROCK"<<"JAZZ"<<"DANGDUT"<<"TRADITIONAL");

    QVariantList l =QVariantList()<<QColor(4278190080).rgba()<< QColor(4280361249).rgba()<<QColor(4294967295).rgba()<< QColor(4278190080).rgba()<<QColor( 4064428610).rgba()<< QColor(4293467747).rgba()<<QColor(4294967295).rgba();

    setDefault("themeColor",l);


    int language = setting.value("language").toInt();
    setting.endGroup();


    QTranslator t;
    if(language!=0)
    {
        t.load(":/usr/share/elroke/languages/elroke_id.qm");
        a.installTranslator(&t);
     }

    mainWindow w;

    w.showFullScreen();
    splash.finish(&w);

    return a.exec();
}
