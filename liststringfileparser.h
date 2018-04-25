#ifndef LISTFILEPARSER_H
#define LISTFILEPARSER_H
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtAlgorithms>

class listStringFileParser
{
public:
    static QList<QString> parse( const QString &file )
    {
        QFile f(file);
        QList<QString>list;

        if(!f.exists())
        {
            qDebug()<<file+" not found";
            return list;
        }

        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
               qDebug()<<"cant read singer";

        QTextStream stream(&f);
        QString line = stream.readLine();
        while(!line.isEmpty())
        {
            list<<line;
            line=stream.readLine();
            qDebug()<<line;
        }

        qSort(list.begin(), list.end());
        return list;
    }
};

#endif // LISTFILEPARSER_H
