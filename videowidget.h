#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include "player.h"

class VideoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent=0);
    ~VideoWidget();
private :
    Player *_player;
public :
    Player *player()const{return _player;};

};

#endif // VIDEOWIDGET_H
