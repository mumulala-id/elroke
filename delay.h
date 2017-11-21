#ifndef DELAY_H
#define DELAY_H

#include <QtCore>

#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>


class delay : public QObject

{
    Q_OBJECT
public:
   explicit delay(QObject *parent=0);

public slots:
    void doDelay();

    void cancelDelay();
    bool stopDelay() const{
        qDebug()<<"checking delay status";
        return m_cancelRequested;}
    bool isWorking() const{
        return working;
    }

private:
     bool m_cancelRequested;

public:
    bool working;

signals:
    void finished();
};

#endif // DELAY_H
