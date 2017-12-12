#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QTranslator>
//#include <QStyleFactory>

int main(int argc, char *argv[])
{
    // turn on the DPI support**
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
QApplication::setStyle("plastique");
    QApplication a(argc, argv);
    QTranslator t;
    t.load(":/usr/share/elroke/indonesia.qm");
    a.installTranslator(&t);

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"SQLITE is not installed";

    //create  database connection
   QSqlDatabase A=  QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase B = QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase C = QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");

    mainWindow w;
    w.showFullScreen();


    return a.exec();
}
