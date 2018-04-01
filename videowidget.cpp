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
    setCursor(Qt::BlankCursor);
       setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

}
void VideoWidget::play()
{
    show();
    _player->play();

}

VideoWidget::~VideoWidget()
{

}
