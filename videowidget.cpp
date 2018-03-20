#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent):
    QWidget(parent)
{
    _player = new Player(this);
    _player->setWinId(this->winId());

       QPalette pal;
       pal.setColor(QPalette::Background,Qt::black);
       pal.setColor(QPalette::Foreground,Qt::white);
       setPalette(pal);

       setWindowFlags( Qt::FramelessWindowHint);
       setWindowState(Qt::WindowFullScreen);

}

