#include "about.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QFile>
#include <QPainter>

about::about(QWidget *parent) :
    QDialog(parent)
{

    QPalette palet;
    palet.setColor(QPalette::Base, palette().dark().color());
    palet.setColor(QPalette::Window, Qt::black);
    palet.setColor(QPalette::WindowText, Qt::white);
    palet.setColor(QPalette::Button, palette().dark().color());

    QVBoxLayout *lo_details = new QVBoxLayout;

    QLabel *label_name_version = new QLabel(this);

    label_name_version->setText("ELROKE");
    label_name_version->setAlignment(Qt::AlignCenter);

    QLabel *description = new QLabel(this);
    description->setText("\tElroke is free and open source apllication for karaoke entertianment. Feel free to use this app for commercial and home use without any fee. But do not forget to  get part for donation to support us. We do not responsible of the contents you used within app");
   description->setWordWrap(true);
    lo_details->addWidget(label_name_version);
    lo_details->addWidget(description);

    QWidget *widget_description = new QWidget(this);
    widget_description->setLayout(lo_details);


    //license
    QTextEdit *license_txt = new QTextEdit(this);

    QFile file(":/usr/share/elroke/file/LICENSE");
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream readfile(&file);

    license_txt->setText(readfile.readAll());
    license_txt->setReadOnly(1);

    QVBoxLayout *lo_main = new QVBoxLayout;

    QPushButton *btn_desc = new QPushButton(tr("Description"), this);
    connect(btn_desc,SIGNAL(pressed()),this,SLOT(showDescription()));

    QPushButton *btn_license = new QPushButton(tr("License"), this);
    connect(btn_license,SIGNAL(pressed()),this,SLOT(showLicense()));

    QPushButton *btn_credit = new QPushButton(tr("Credit"), this);
    connect(btn_credit,SIGNAL(pressed()),this,SLOT(showCredit()));

    QPushButton *btn_donatur =new QPushButton(tr("Donatur"), this);
    connect(btn_donatur,SIGNAL(pressed()),this,SLOT(showDonatur()));

    QPushButton *btn_close = new QPushButton(tr("Close"), this);
    connect(btn_close,SIGNAL(clicked(bool)),this,SLOT(close()));

    //Credit

    QTextEdit *credit_text = new QTextEdit(this);

    credit_text->setText("Thank to all contributor :\n\n"
                "Muhammad Mukharom(mumulala)\n"
                         //keep my name first :D
                         "you");
    credit_text->setReadOnly(1);

    QTextEdit * donatur = new QTextEdit (this);

    donatur->setText("Thank to all donatur, keep Elroke alive :\n\n"
                     "John\n"
                     "Due\n");
    donatur->setReadOnly(1);

    QHBoxLayout *lo_btn = new QHBoxLayout;
    lo_btn->addWidget(btn_desc);
    lo_btn->addWidget(btn_license);
    lo_btn->addWidget(btn_credit);
    lo_btn->addWidget(btn_donatur);
    lo_btn->addWidget(btn_close);

    stack = new QStackedLayout;

    stack->addWidget(widget_description);
    stack->addWidget(license_txt);
    stack->addWidget(credit_text);
    stack->addWidget(donatur);



       lo_main->addLayout(stack);
       lo_main->addLayout(lo_btn);

    setLayout(lo_main);

    setFixedSize(800,360);
//    setWindowTitle("About Elroke");
    setWindowFlags(Qt::FramelessWindowHint);
    setPalette(palet);


}

void about::showDescription(){

    stack->setCurrentIndex(0);

}

void about::showLicense(){

    stack->setCurrentIndex(1);

}

void about::showCredit(){

    stack->setCurrentIndex(2);

}

void about::showDonatur(){

    stack->setCurrentIndex(3);
}

