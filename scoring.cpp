#include "scoring.h"
#include <QTime>
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>
#include <QPainter>
Scoring::Scoring(QWidget *parent) : QDialog(parent)
{
    auto *layout = new QVBoxLayout;
    qsrand(static_cast<uint>(QTime::currentTime().msec()));


    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    QFont font;
    font.setPointSize(156);
    font.setBold(1);

    label = new QLabel(this);
    label->setFont(font);

    timer = new QTimer(this);
    timer->setSingleShot(1);
    timer->setInterval(500);
    timer_random = new QTimer(this);
    timer_random->setInterval(10);

    player = new Player(this);
    QFile file("/usr/share/elroke/soundfx/applause.mp3");
    if(!file.exists())
        finish();
    player->setFile(file.fileName());

    connect(timer,&QTimer::timeout,player,&Player::play);
    connect(player,&Player::reachEnded,this,&Scoring::finish);
    connect(timer,&QTimer::timeout,timer_random,&QTimer::stop);
    connect(timer_random,&QTimer::timeout,[this]()
    {
        label->setText(QString::number((qrand()%46)+50));
    });
    layout->addWidget(label);
    setLayout(layout);
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::blue);
    pal.setColor(QPalette::Window,Qt::white);

     setPalette(pal);
    hide();
}


void Scoring::start(){
    show();
    timer_random->start();
    timer->start();
}

void Scoring::finish()
{
    hide();
    emit finished();

}

