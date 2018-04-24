#include "themewidget.h"
#include <QPainter>
themewidget::themewidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(32,32);
}

void themewidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    painter.setBrush(primaryColor));
    int w = width()/2;
    painter.drawRect(0,0,w,w);
    painter.setBrush(QColor("#EC407A"));
    painter.drawRect(w,w,w,w);
    painter.setBrush(Qt::green);
    painter.drawRect(0,w,w,w);
    painter.setBrush(Qt::yellow);
    painter.drawRect(w,0,w,w);
    painter.setBrush(Qt::black);
    painter.drawRect(w/2,w/2,w,w);



}
