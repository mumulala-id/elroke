#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent):
    QWidget(parent)
{

//

        QFont fontTitle;
        fontTitle.setPointSize(32);

        QFont fontSinger;
        fontSinger.setPointSize(56);

        label_title = new QLabel(this);
        label_title->setFont(fontTitle);

        label_singer = new QLabel(this);
        label_singer->setFont(fontSinger);

        singer_animation = new QPropertyAnimation(label_singer, "geometry", this);
        singer_animation->setDuration(500);

        title_animaton = new QPropertyAnimation(label_title, "geometry", this);
        title_animaton->setDuration(500);

        _song = new Song(this);

        QPalette let;
        let.setColor(QPalette::Background,Qt::gray);
        let.setColor(QPalette::Foreground,Qt::black);
        setPalette(let);

        _player = new Player(this);
        _player->setWinId(this->winId());


        QPalette pal;
        pal.setColor(QPalette::Background,Qt::black);
        pal.setColor(QPalette::Foreground,Qt::white);
        setPalette(pal);
        setCursor(Qt::BlankCursor);
        setWindowFlags( Qt::FramelessWindowHint);

}

void VideoWidget::setFullscreenMode()
{
       resize(QSize(desktop_width,desktop_height));
}
void VideoWidget::play()
{
    if(_player->isPausing() || _player->isPlaying())
        _player->stop();
    showFullScreen();

    singer_animation->setStartValue(QRect(0,0, width(),height()-label_singer->height()));
    singer_animation->setEndValue(QRect((width()-label_singer->width())/2 ,0, width(), height()-label_singer->height()));
    title_animaton->setStartValue(QRect(width(), 80, width(), height()));
    title_animaton->setEndValue(QRect((width()-label_title->width())/2, 80, width(), height()));

    singer_animation->start();
    title_animaton->start();

    QTimer::singleShot(4000,_player,SLOT(play()));

//    _player->play();

}

void VideoWidget::setData(Song *song){
    _song = song;
      label_title->setText(song->getTitle());
       label_title->adjustSize();
        label_singer->setText(song->getSinger());
         label_singer->adjustSize();
         _player->setFile(song->getPath());

}

VideoWidget::~VideoWidget()
{

}
