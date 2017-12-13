
#include "opening.h"

#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>
#include <QDebug>

opening::opening(QString t, QString s, QWidget *parent) : QDialog(parent), title(t), singer(s)
{

//    resize(parent->size());
  resize(QSize(desktop_width,desktop_height));
    QFont fontTitle;
    fontTitle.setPointSize(32);

    QFont fontSinger;
    fontSinger.setPointSize(56);

    auto *label_title = new QLabel(this);
    label_title->setText(title);
    label_title->setFont(fontTitle);
    label_title->adjustSize();

    auto *label_singer = new QLabel(this);
    label_singer->setText(singer);
    label_singer->setFont(fontSinger);
    label_singer->adjustSize();

    auto *animation = new QPropertyAnimation(label_singer, "geometry", this);
    animation->setDuration(800);
     animation->setStartValue(QRect(0,0, width(),height()));
     animation->setEndValue(QRect((width()-label_singer->width())/2 ,0, width(), height()));
     animation->start();

    auto *animation1 = new QPropertyAnimation(label_title, "geometry", this);
    animation1->setDuration(800);
    animation1->setStartValue(QRect(width(), 80, width(), height()));
    animation1->setEndValue(QRect((width()-label_title->width())/2, 80, width(), height()));
    animation1->start();

    QPalette let;
    let.setColor(QPalette::Background,Qt::gray);
    let.setColor(QPalette::Foreground,Qt::black);

    setPalette(let);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);

}

