#include "songitemwidget.h"
#include <QVBoxLayout>
songitemwidget::songitemwidget(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout *main_layout = new QVBoxLayout;

    label_title = new QLabel(this);

//    label_title->sete
    QFont font_small;
    font_small.setPointSize(10);

    label_singer = new QLabel(this);
    label_singer->setFont(font_small);
    song_ = new Song(this);


    main_layout->addWidget(label_title,1,Qt::AlignLeft);
    main_layout->addWidget(label_singer,1,Qt::AlignRight);

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
