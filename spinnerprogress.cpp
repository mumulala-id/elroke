#include "spinnerprogress.h"
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QDebug>
#include <QTransform>

spinnerProgress::spinnerProgress(QWidget *parent)
    : QWidget(parent)
{


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);

//    QPalette pal;
//    pal.setColor(QPalette::Background,Qt::transparent);
//    setPalette(pal);
    setWindowTitle(tr("Analog Clock"));
    resize(100, 100);
//    setMinimumSize(100,100);
}

void spinnerProgress::setValue(uint val)
{
    value = val;
    if(value!=0.0)
        play=true;
}

void spinnerProgress::start()
{
    play =true;
}

void spinnerProgress::stop()
{
    play = false;
}
void spinnerProgress::paintEvent(QPaintEvent *)
{
    QTime time = QTime::currentTime();
    QPainter painter(this);
    int startAngle = -270 * 16;
    int spanAngle = value*57.6;

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPen pen(Qt::white,2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin);
    painter.setPen(pen);
    painter.translate(width() / 2, height() / 2);
    painter.drawArc(-width()/2,-height()/2,width(),height(),startAngle,spanAngle);
    painter.save();
    QTransform transform(1,0,0,1,width()/2, height()/2);
    QPixmap pix(":/usr/share/elroke/icon/disk.png");
    pix = pix.scaled(width()-8,height()-8,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    if(play)
    transform.rotate(90*time.second() );
    painter.setTransform(transform);
    painter.drawPixmap(-pix.width()/2, -pix.height()/2,pix);
    painter.restore();

}
