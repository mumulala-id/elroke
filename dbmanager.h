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
     dbmanager(dbcontype contype, QObject *parent=nullptr);
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
    void updatePlayedTime(const QString &id);
    void updatePath(const QString &id, const QString &path);
    Song *getSong(const QString &id);
    void setFavorite(const QString &id);
    bool isFavorite(const QString &id);

private slots:
    bool restoreDB();
    bool openDB();
    bool createTable();

public slots:

};

#endif // DBMANAGER_H
