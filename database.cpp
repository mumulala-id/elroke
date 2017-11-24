//#include "database.h"
//#include <QtSql/QSqlDriver>
//#include <QtSql/QSqlError>
//#include <QtSql/QSqlQuery>
//#include <QDebug>
//#include <QFile>
//#include <QDir>
//#include <QMap>


//database::database(QObject *parent)
//    : QObject(parent)
//{
//    if(!QSqlDatabase::isDriverAvailable("QSQLITE"))
//        qDebug()<<"sqlite is not installed";

//    //db = QSqlDatabase::addDatabase("QSQLITE");
//    QSqlDatabase firstDB = QSqlDatabase::addDatabase("QSQLITE", "first");
//   QSqlDatabase secondDB = QSqlDatabase::addDatabase("QSQLITE","second");


////    db = QSqlDatabase::database();
////    firstDB = QSqlDatabase::database("first");
////    secondDB = QSqlDatabase::database("second");
//    QString dir_env = QDir::homePath()+"/.config/elroke";

//    if(!QDir(dir_env).exists())
//         QDir().mkpath(dir_env);

//    QString dbname = dir_env+"elroke.db";

////    db.setDatabaseName(dbname);
//    firstDB.setDatabaseName(dbname);
//    secondDB.setDatabaseName(dbname);

//}

//void database::connectToDB(int i){


//    if(!QFile(QDir::homePath()+"/.config/elroke/elroke.db").exists()){
//        this->restoreDB();
//        qDebug()<<"restore database";
//    }
//    else{
//        this->openDB(i);
//        qDebug()<<"db opened";
//    }
//}

//bool database::restoreDB(){
//    if(this->openDB(1)){
//        if(!this->createTable())
//            return false;
//        else
//            return true;
//    }

//    return false;

//}

//bool database::openDB(int in){
//    QSqlDatabase db = QSqlDatabase::database();

//    QSqlDatabase firstDB = QSqlDatabase::database("first");
//    QSqlDatabase secondDB=QSqlDatabase::database("second");


//    switch (in) {
//    case 1:
//        if(!firstDB.open())
//            return false;

//        break;
//    case 2:
//        if(!secondDB.open())
//            return false;
//    default:
//        break;
//    }



//if(!db.open())
//    return false;



//       return true;
//}

//bool database::createTable(){

//     QSqlQuery query;
//    query.prepare("CREATE TABLE IF NOT EXISTS xelroke_table (id INTEGER UNIQUE PRIMARY KEY, title TEXT, singer TEXT, language TEXT, category TEXT,channel TEXT, path TEXT)");

//    if(!query.exec()){
//        qDebug()<<"ERROR 2:"<<db.lastError().text();
//        return false;
//    }
//    else {
//        qDebug()<<"table created";
//        return true;
//    }

//    return false;
//}



//void database::closeDB(){
//    db.close();

//}

//database::~database()
//{

//}


//bool database::begin(){

//     return db.database().transaction();

//}

//bool database::commet(){

//    return db.database().commit();

//}

