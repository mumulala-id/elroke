/*
    ElRoke
    Copyright (C) Muhammad Mukharom

    This file is part of ElRoke.
    ElRoke is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ElRoke is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with ElRoke.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "opening.h"

#include <QDebug>
#include <QTimer>

opening::opening( QWidget *parent) : QWidget(parent)
{

  resize(QSize(desktop_width,desktop_height));

    QFont fontTitle;
    fontTitle.setPointSize(32);

    QFont fontSinger;
    fontSinger.setPointSize(56);

    label_title = new QLabel(this);
    label_title->setFont(fontTitle);

    label_singer = new QLabel(this);
    label_singer->setFont(fontSinger);

    singer_animation = new QPropertyAnimation(label_singer, "geometry", this);
    singer_animation->setDuration(1200);

    title_animaton = new QPropertyAnimation(label_title, "geometry", this);
    title_animaton->setDuration(1200);

    QPalette let;
    let.setColor(QPalette::Background,Qt::gray);
    let.setColor(QPalette::Foreground,Qt::black);

    setPalette(let);
    setWindowFlags( Qt::FramelessWindowHint);
    setWindowState(Qt::WindowFullScreen);
//    setAutoFillBackground(1);
    hide();


}


void opening::setData(QString title, QString singer){
      label_title->setText(title);
       label_title->adjustSize();
        label_singer->setText(singer);
         label_singer->adjustSize();

}


void opening::start(){

    show();
    raise();
    singer_animation->setStartValue(QRect(0,0, width(),height()-label_singer->height()));
    singer_animation->setEndValue(QRect((width()-label_singer->width())/2 ,0, width(), height()-label_singer->height()));


    title_animaton->setStartValue(QRect(width(), 80, width(), height()));
    title_animaton->setEndValue(QRect((width()-label_title->width())/2, 80, width(), height()));

    singer_animation->start();
    title_animaton->start();

    QTimer::singleShot(4000,this,SLOT(finish()));
}

void opening::finish(){

    hide();
    emit passed();


}
