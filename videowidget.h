#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "player.h"
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QApplication>
#include <QLabel>

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent=0);
    ~VideoWidget();
private :
    Player *_player;
    QString _totle;
    QString _singer;
    QDesktopWidget *desktop = QApplication::desktop();
    unsigned short int desktop_width = desktop->width();
    unsigned short int desktop_height = desktop->height();
    QLabel *label_title;
    QLabel *label_singer;
    QPropertyAnimation *title_animaton;
    QPropertyAnimation *singer_animation;

public :
    Player *player()const{return _player;};
    void play();
    void setData(QString title,QString singer);
};

#endif // VIDEOWIDGET_H
