#include "delay.h"
#include <QDebug>

delay::delay(QObject *parent) :
    QObject(parent),
    m_cancelRequested(false)
    //m_pauseRequired(false)
{

}

void delay::doDelay(){
    working=true;
qDebug()<<"before"<<isWorking();
    for (int i=5;i>0;i--){
        if (stopDelay()) {
            working=false;
            qDebug()<<"cancelled";
            return;
        }
        QThread::sleep(1);
        qDebug()<<i;
    }



           working=false;
           qDebug()<<"after"<<isWorking();
           emit finished();
           qDebug()<<"emitted";
}


void delay::cancelDelay()
{
qDebug()<<"cancel aceepeted";

   m_cancelRequested = true;
}
