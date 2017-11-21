#include "dbmanager.h"
#include <QDebug>
#include <QVariant>
#include <QSqlQuery>
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
    case playlist:
        conname = "elroke_playlist";
        break;

    }

    db = QSqlDatabase::database(conname);

    }


void dbmanager::connectToDB(){

    if(!QFile(dbname).exists()){

        restoreDB();

    }
    else{

qDebug()<<"db opened";
        openDB();
    }

}

bool dbmanager::restoreDB(){

    if(!QDir(dbdir).exists())
        QDir().mkpath(dbdir);

    if(openDB()){

        if(!createTable())
            return false;
        return true;

    }

        return false;

}

bool dbmanager::openDB(){

  if(!db.open()){
      qDebug()<<"Database not open";
      return false;
  }

  return true;

}


bool dbmanager::createTable(){

QSqlQuery query(db);

//create playlist table
    if(conname=="elroke_playlist"){
        query.prepare("CREATE TABLE IF NOT EXISTS playlist123 (ID INTEGER UNIQUE PRIMARY KEY, PATH TEXT,TITLE TEXT, SINGER TEXT, CHANNEL TEXT)");

    }
    else
    query.prepare("CREATE TABLE IF NOT EXISTS elroke123 (ID INTEGER UNIQUE PRIMARY KEY, TITLE TEXT, SINGER TEXT, LANGUAGE TEXT, CATEGORY TEXT,CHANNEL TEXT, PATH TEXT)");

    if(query.exec()){
        qDebug()<<"table created";
        return true;
    }
    qDebug()<<"table not created";
    return false;


}

void dbmanager::closeDB(){

    db.close();
}
void dbmanager::prepare(){

    db.transaction();
}


void dbmanager::submit(){
    db.commit();
}

void dbmanager::setDBName(const QString &name){

    dbname=dbdir+"/"+name+".db";
    db.setDatabaseName(dbname);

}

void dbmanager::setDBDir(const QString &dirname){

    dbdir = QDir::homePath()+"/.config/elroke/"+dirname;


}

QString dbmanager::dbName(){
    return dbname;

}

void dbmanager::rollBack(){
    db.rollback();
}

bool dbmanager::insertIntoTable(const QVariantList &data){

    QSqlQuery  query(db);

    if(conname=="elroke_playlist"){
query.prepare("INSERT INTO playlist123 (PATH,  PLAYLIST, SINGER,  CHANNEL  ) VALUES (:Path, :Title, :Singer, :Channel )");
query.bindValue(":Path", data[0].toString());
query.bindValue(":Title", data[1].toString());
query.bindValue(":Singer", data[2].toString());
 query.bindValue(":Channel", data[3].toString());
}
else{

   query.prepare("INSERT INTO elroke123 (  TITLE , SINGER, LANGUAGE , CATEGORY, CHANNEL, PATH  ) VALUES (:Title, :Singer, :Language, :Category, :Channel, :Path )");

   query.bindValue(":Title", data[0].toString());
   query.bindValue(":Singer", data[1].toString());
    query.bindValue(":Language", data[2].toString());
     query.bindValue(":Category", data[3].toString());
     query.bindValue(":Channel", data[4].toString());
     query.bindValue(":Path", data[5].toString());
}
     if(!query.exec())
         return false;
     return true;

}
