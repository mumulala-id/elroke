#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
            qDebug()<<"sqlite is not installed";

    //create  database connection
    QSqlDatabase A=  QSqlDatabase::addDatabase("QSQLITE","elroke_show");
    QSqlDatabase B = QSqlDatabase::addDatabase("QSQLITE","elroke_add");
    QSqlDatabase C = QSqlDatabase::addDatabase("QSQLITE", "elroke_edit");

    mainWindow w;
w.showFullScreen();


    return a.exec();
}
