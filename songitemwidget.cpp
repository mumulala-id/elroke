#include "songitemwidget.h"
#include <QVBoxLayout>
#include <QDebug>



songitemwidget::songitemwidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *main_layout = new QHBoxLayout;
    QVBoxLayout *sub_main_layout = new QVBoxLayout;

    label_title = new QLabel(this);

    QFont font_small;
    font_small.setPointSize(12);

    label_singer = new QLabel(this);
    label_singer->setFont(font_small);
    song_ = new Song(this);

    sub_main_layout->addWidget(label_title,1,Qt::AlignLeft);
    sub_main_layout->addWidget(label_singer,1,Qt::AlignRight);

    QLabel *label_drag = new QLabel(this);
    label_drag->setPixmap(QPixmap(":/usr/share/elroke/icon/drag.png"));
    label_drag->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    main_layout->addLayout(sub_main_layout);
    main_layout->addWidget(label_drag,0);
    main_layout->setSpacing(4);
    QPalette pal;
    pal.setColor(QPalette::Text,QColor(0,0,0,128));
    pal.setBrush(QPalette::HighlightedText,Qt::red);
    setPalette(pal);
    setLayout(main_layout);
}

void songitemwidget::setSong(Song * _song)
{
    song_ = _song;
    label_title->setText(song_->getTitle());
    label_singer->setText(song_->getSinger());
}

Song *songitemwidget::song()
{
    return song_;
}
