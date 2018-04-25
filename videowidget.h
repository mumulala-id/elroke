#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "player.h"
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QApplication>
#include <QLabel>
#include <song.h>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent=nullptr);
    ~VideoWidget();
private :
    Player *_player;
//    QString _title;
//    QString _singer;
    QDesktopWidget *desktop = QApplication::desktop();
    unsigned short int desktop_width = desktop->width();
    unsigned short int desktop_height = desktop->height();
    QLabel *label_title;
    QLabel *label_singer;
    QPropertyAnimation *title_animaton;
    QPropertyAnimation *singer_animation;
    Song *_song;

public :
    Player *player()const{return _player;};
    void play();
    void setData(Song *);
    void setFullscreenMode();
    Song *song()const{return _song;};
};

#endif // VIDEOWIDGET_H
