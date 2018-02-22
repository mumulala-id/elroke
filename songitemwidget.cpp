#include "songitemwidget.h"
#include <QVBoxLayout>

songitemwidget::songitemwidget(QWidget *parent) : QWidget(parent)
{


    QHBoxLayout *main_layout = new QHBoxLayout;
    QVBoxLayout *sub_main_layout = new QVBoxLayout;
    setAutoFillBackground(1);

    label_title = new QLabel(this);

//    label_title->sete
    QFont font_small;
    font_small.setPointSize(10);

    label_singer = new QLabel(this);
    label_singer->setFont(font_small);
    song_ = new Song(this);


    sub_main_layout->addWidget(label_title,1,Qt::AlignLeft);
    sub_main_layout->addWidget(label_singer,1,Qt::AlignRight);

    drag_icon = new QPushButton(QIcon(":/usr/share/elroke/file/icon/drag.png"),"",this);
    drag_icon->setFlat(1);
    drag_icon->setFocusPolicy(Qt::NoFocus);
    drag_icon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    main_layout->addLayout(sub_main_layout);
    main_layout->addWidget(drag_icon,0);
    main_layout->setSpacing(0);
//    main_layout->setMargin(0);

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::green);
    pal.setColor(QPalette::WindowText,Qt::yellow);
setPalette(pal);
    setLayout(main_layout);

}

void songitemwidget::setSong(Song * _song){

    song_ = _song;

    label_title->setText(song_->getTitle());
    label_singer->setText(song_->getSinger());

}

Song *songitemwidget::song(){

    return song_;

}

QPushButton * songitemwidget::dragButton(){

    return drag_icon;
}
