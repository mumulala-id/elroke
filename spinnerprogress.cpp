#include "spinnerprogress.h"
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QDebug>
#include <QTransform>

spinnerProgress::spinnerProgress(QWidget *parent)
    : QWidget(parent)
{


    auto timer = new QTimer(this);
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
       QBrush fillBrush = QBrush(Qt::white, Qt::SolidPattern);
       QPen markPen(Qt::black);
       QPoint centrePt(this->width() / 2, this->height() / 2);
       int arcWidth = 8;
       int radius = (this->width() / 2) - arcWidth;
       int angleSpan_ = 57.6*value;
       int startAngle_ = -270 * 16;

       QRect bounds(centrePt.x()-radius, centrePt.y()-radius, (2*radius), (2*radius));


       painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
       painter.setBrush(Qt::transparent);

       // Draw the circle
       QPen pen(QColor(0,0,0,80),8,Qt::SolidLine);
       painter.setPen(pen);

       fillBrush = QBrush(Qt::black, Qt::SolidPattern);

       painter.drawEllipse(centrePt, radius, radius);


           //  Draw Arc
           markPen.setCapStyle(Qt::FlatCap);
           markPen.setJoinStyle(Qt::RoundJoin);
           markPen.setStyle(Qt::SolidLine);
           markPen.setColor(Qt::white);
           markPen.setWidth(arcWidth);
           painter.setPen(markPen);
           painter.drawArc(bounds, startAngle_, angleSpan_);
           painter.save();

           QTransform transform(1,0,0,1,width()/2, height()/2);
           QPixmap pix(":/usr/share/elroke/icon/disk.png");
           pix = pix.scaled(radius*2,radius*2,Qt::KeepAspectRatio,Qt::SmoothTransformation);
           if(play)
           transform.rotate(90*time.second() );
           painter.setTransform(transform);
           painter.drawPixmap(-pix.width()/2, -pix.height()/2,pix);

           painter.restore();

           painter.end();

}
