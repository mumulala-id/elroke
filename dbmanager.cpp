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
    qDebug()<<dbdir;
    dbname = dbdir+"/elroke.db";

    db = QSqlDatabase::database(conname);
//    QSqlQuery query(db);
    db.setDatabaseName(dbname);

}

void dbmanager::connectToDB(){

    if(!QFile(dbname).exists()){

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

Song* dbmanager::getSong(int id){
    qDebug()<<"id"<<id;
    QSqlQuery query(db);
    
    QSqlRecord rec;
    query.prepare("SELECT * FROM ELROKE123 WHERE ID="+QString::number(id));
    
    if(query.exec()){
            while(query.next()){
                rec = query.record();
                
            }
    }

////            for(int i=0; rec.count();i++){
////            qDebug()<<rec.value(i).toString();
//////            qDebug()<<rec.value(1).toString();
////            }
            
            
            
//}

////    qDebug()<<rec.count();
//    for(int i=0; i<rec.count();i++){
//               qDebug()<<i<<rec.value(i).toString();
////               qDebug()<<rec.value(1).toString();
//}
    
    Song *the_song  = new Song;
    the_song->setId(rec.value(0).toInt());
    the_song->setTitle(rec.value(1).toString());
    the_song->setSinger(rec.value(2).toString());
    the_song->setLanguage(rec.value(3).toString());
    the_song->setCategory(rec.value(4).toString());
    the_song->setPlaytimes(rec.value(6).toInt());
    the_song->setPath(rec.value(7).toString());
QString channel = rec.value(5).toString();

if(QString::compare(channel, "LEFT", Qt::CaseInsensitive)==0){

    the_song->setAudioChannel(Song::audioChannel::left);

}
else if(QString::compare(channel, "RIGHT", Qt::CaseInsensitive)==0){
 the_song->setAudioChannel(Song::audioChannel::right);
}
 else{
     the_song->setAudioChannel(Song::audioChannel::stereo);

}

    return the_song;
    
    
    
}
