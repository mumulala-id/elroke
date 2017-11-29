#include "dbmanager.h"
#include <QDebug>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
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


    db = QSqlDatabase::database(conname);
    QSqlQuery query(db);
    db.setDatabaseName(dbname);

}

void dbmanager::connectToDB(){

    if(!QFile(QDir::homePath()+"/.elroke/elroke.db").exists()){

        restoreDB();

    }
    else{

        openDB();
    }

}

bool dbmanager::restoreDB(){

    if(!QDir(dbdir).exists())
        QDir().mkpath(dbdir);

    if(openDB()){

        if(!createTable()){
            qDebug()<<"cant create table"<<db.lastError();
            return false;
        }
        return true;

    }

        return false;

}

bool dbmanager::openDB(){

  if(!db.open()){
      qDebug()<<"Database not open"<<db.lastError();
      return false;
  }

  return true;

}


bool dbmanager::createTable(){

QSqlQuery query(db);

//create playlist table

    query.prepare("CREATE TABLE IF NOT EXISTS ELROKE123 (ID INTEGER UNIQUE PRIMARY KEY, TITLE TEXT, SINGER TEXT, LANGUAGE TEXT, CATEGORY TEXT,CHANNEL TEXT, PLAYTIMES INT, PATH TEXT , DATE TEXT)");

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


QString dbmanager::dbName(){
    return dbname;

}

void dbmanager::rollBack(){
    db.rollback();
}

bool dbmanager::insertIntoTable(const QVariantList &data){

    QSqlQuery  query(db);

   query.prepare("INSERT INTO ELROKE123 (  TITLE , SINGER, LANGUAGE , CATEGORY, CHANNEL, PLAYTIMES, PATH, DATE  ) VALUES (:Title, :Singer, :Language, :Category, :Channel, :Playtimes, :Path , :Date)");

     query.bindValue(":Title", data[0].toString());
     query.bindValue(":Singer", data[1].toString());
     query.bindValue(":Language", data[2].toString());
     query.bindValue(":Category", data[3].toString());
     query.bindValue(":Channel", data[4].toString());
     query.bindValue(":Playtimes", 0);
     query.bindValue(":Path", data[5].toString());
     query.bindValue(":Date", QDate::currentDate().toString("yyyy-MM-dd"));

     if(!query.exec())
         return false;
     return true;

}

void dbmanager::updatePlayedTime(int id){

    QSqlQuery query(db);
  int value=0;
    query.prepare("SELECT PLAYTIMES FROM ELROKE123 WHERE ID = "+QString::number(id));

    if(query.exec()){
        query.first();
          value = query.value(0).toInt()+1;
    }
    query.clear();
    prepare();

    query.prepare("UPDATE ELROKE123 SET PLAYTIMES ="+QString::number(value)+" WHERE ID = "+QString::number(id));
    if(!query.exec())
        qDebug()<<"cant update played time";
    else
        submit();

query.clear();




}
