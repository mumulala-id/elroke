#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QSqlRecord>
dbmanager::dbmanager(dbcontype contype, QObject *parent)
{
    Q_UNUSED(parent)

    switch (contype) {
    case show:
        conname = "elroke_show";
        break;
    case edit :
        conname = "elroke_edit";
        break;
    case add :
        conname = "elroke_add";
        break;

    }

    dbdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    dbname = dbdir+"/elroke.db";

    db = QSqlDatabase::database(conname);
    db.setDatabaseName(dbname);
}

void dbmanager::connectToDB()
{
    if(!QFile(dbname).exists())
    {
        restoreDB();
    }
    else
    {
        openDB();
    }
}

bool dbmanager::restoreDB()
{
    if(!QDir(dbdir).exists())
        QDir().mkpath(dbdir);

    if(openDB())
    {
        if(!createTable())
        {
            qDebug()<<"cant create table"<<db.lastError();
            return false;
        }
        return true;
    }
        return false;
}

bool dbmanager::openDB()
{
    if(!db.open())
    {
      qDebug()<<"Database not open"<<db.lastError();
      return false;
    }
     return true;
}

bool dbmanager::createTable()
{
    QSqlQuery query(db);
    query.prepare("CREATE TABLE IF NOT EXISTS ELROKE123 (ID INTEGER UNIQUE PRIMARY KEY, TITLE TEXT, SINGER TEXT, LANGUAGE TEXT, GENRE TEXT,CHANNEL TEXT, PLAYTIMES INT, PATH TEXT NOT NULL , DATE TEXT, FAVORITE TEXT, UNIQUE (PATH))");

    if(query.exec())
        return true;

    return false;
}

void dbmanager::closeDB()
{
    db.close();
}
void dbmanager::prepare()
{
    db.transaction();
}

void dbmanager::submit()
{
    db.commit();
}

QString dbmanager::dbName()
{
    return dbname;
}

void dbmanager::rollBack()
{
    db.rollback();
}

bool dbmanager::insertIntoTable(const QVariantList &data)
{
    QSqlQuery  query(db);
    query.prepare("INSERT INTO ELROKE123 (  TITLE , SINGER, LANGUAGE , GENRE, CHANNEL, PLAYTIMES, PATH, DATE, FAVORITE ) VALUES (:Title, :Singer, :Language, :Genre, :Channel, :Playtimes, :Path , :Date, :Favorite)");

     query.bindValue(":Title", data[0].toString());
     query.bindValue(":Singer", data[1].toString());
     query.bindValue(":Language", data[2].toString());
     query.bindValue(":Genre", data[3].toString());
     query.bindValue(":Channel", data[4].toString());
     query.bindValue(":Playtimes", 0);
     query.bindValue(":Path", data[5].toString());
     query.bindValue(":Date", QDate::currentDate().toString("yyyy-MM-dd"));
     query.bindValue(":Favorite", "NO");

     if(!query.exec())
         return false;
     return true;
}

void dbmanager::updatePlayedTime(const QString &id)
{
    QSqlQuery query(db);
    uint value=0;
    query.prepare("SELECT PLAYTIMES FROM ELROKE123 WHERE ID = "+id);

    if(query.exec())
    {
        query.first();
        value = query.value(0).toInt()+1;
    }
    query.clear();
    query.prepare("UPDATE ELROKE123 SET PLAYTIMES ="+QString::number(value)+" WHERE ID = "+id);

    if(!query.exec())
        qDebug()<<"cant update played time";
}

void dbmanager::updatePath(const QString &id, const QString &path)
{
    QSqlQuery query(db);
     query.prepare("UPDATE ELROKE123 SET PATH ="+path+" WHERE ID = "+id);

     if(!query.exec())
         qDebug()<<"cant update path";
}

Song* dbmanager::getSong(const QString &id)
{
    QSqlQuery query(db); 
    QSqlRecord rec;
    query.prepare("SELECT * FROM ELROKE123 WHERE ID="+id);
    
    if(query.exec())
    {
            while(query.next())
               rec = query.record();
    }

   if(!query.first())//check valid
   {
       qDebug()<< "song" << id <<"not found";
        return nullptr;
   }
    
    Song *the_song  = new Song;
    the_song->setId(rec.value(0).toString());
    the_song->setTitle(rec.value(1).toString());
    the_song->setSinger(rec.value(2).toString());
    the_song->setLanguage(rec.value(3).toString());
    the_song->setCategory(rec.value(4).toString());
    the_song->setPlaytimes(rec.value(6).toInt());
    the_song->setPath(rec.value(7).toString());
    the_song->setAudioChannel(rec.value(5).toString());

    bool fav = QString::compare(rec.value(9).toString(),"YES",Qt::CaseInsensitive)==0;
    the_song->setFavorite(fav);

    return the_song;
}
void dbmanager::setFavorite(const QString &id)
{
    QSqlQuery query(db);
    QString f;
    if (isFavorite(id))
    {
        f = "NO";
    }  else {
        f = "YES";
    }
     query.prepare("UPDATE ELROKE123 SET FAVORITE =? WHERE ID = "+id);
     query.addBindValue(f);
     if (!query.exec())
     {
         qDebug()<<"Can't set favorite";
         qDebug()<<db.lastError();
     }  else    {
         qDebug()<<"update favorite";
     }
}

bool dbmanager::isFavorite(const QString &id)
{
    QSqlQuery query(db);
    query.prepare("SELECT FAVORITE FROM ELROKE123 WHERE ID = "+id);

    if(!query.exec())
        return false;

    query.first();
    QString v =query.record().value(0).toString();

    if(v == "YES")
        return true;
    return false;
}

dbmanager::~dbmanager()
{
    db.close();
}
