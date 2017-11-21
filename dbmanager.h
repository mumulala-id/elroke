#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QObject>
#include <QSqlDatabase>
#include <QDir>
#include <QVariantList>

class dbmanager
{

public:
    enum dbcontype{ show, edit, add, playlist};
     dbmanager(dbcontype contype, QObject *parent=0);

private:

    dbcontype contype;
    QSqlDatabase db;
   QString dbname;
    QString dbdir = QDir::homePath()+"/.config/elroke";
    QString conname="";

public slots:
    void connectToDB();
    QSqlDatabase database(){return db;}
    void setDBName(const QString &);
    void setDBDir(const QString &);
    void prepare();
    void submit();
     QString dbName();
      void closeDB();
      void rollBack();
      bool insertIntoTable(const QVariantList & );


private slots:


    bool restoreDB();
    bool openDB();
    bool createTable();




public slots:

};

#endif // DBMANAGER_H
