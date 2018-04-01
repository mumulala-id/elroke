#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QDir>
#include <QVariantList>
#include "song.h"
#include <QStandardPaths>

class dbmanager
{

public:
    enum dbcontype{ show, edit, add};
     dbmanager(dbcontype contype, QObject *parent=0);
     ~dbmanager();

private:

    dbcontype contype;
    QSqlDatabase db;
    QString dbname;
    QString dbdir;
    QString conname="";

public slots:
    void connectToDB();
    QSqlDatabase database(){return db;}
    void prepare();
    void submit();
    QString dbName();
    void closeDB();
    void rollBack();
    bool insertIntoTable(const QVariantList & );
    void updatePlayedTime(int id);
    Song *getSong(int id);

private slots:
    bool restoreDB();
    bool openDB();
    bool createTable();

public slots:

};

#endif // DBMANAGER_H
