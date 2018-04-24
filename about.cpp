/*
 *
    ElRoke
    Copyright (C) Muhammad Mukharom

     This file is part of ElRoke.
    ElRoke is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ElRoke is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with ElRoke.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "about.h"
#include <QApplication>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>
#include <QFile>
#include <QDesktopServices>
#include <QTabBar>

about::about(QWidget *parent) :
    QDialog(parent)
{
    QPalette palet;
    palet.setColor(QPalette::Base, Qt::white);
    palet.setColor(QPalette::Window, Qt::white);
    palet.setColor(QPalette::Text,QColor(0,0,0,128));
    palet.setColor(QPalette::WindowText, QColor(0,0,0,128));
    palet.setColor(QPalette::Button, palette().dark().color());
    palet.setColor(QPalette::ButtonText,Qt::white);

    auto lo_main = new QVBoxLayout;

    auto tab = new QTabBar(this);
    tab->addTab(tr("Description"));
    tab->addTab(tr("License"));
    tab->addTab(tr("Credit"));
    tab->addTab(tr("Donatur"));

    auto lo_details = new QVBoxLayout;

    auto label_appname = new QLabel(this);

    label_appname->setText(QApplication::applicationName());
    label_appname->setAlignment(Qt::AlignCenter);

    auto label_version = new QLabel(this);
    label_version->setText(QApplication::applicationVersion());
    label_version->setAlignment(Qt::AlignCenter);

    auto description = new QLabel(this);
    description->setText("\tElroke is free and open source apllication for karaoke entertainment. Feel free to use this app for commercial and home use without any fee. But do not forget to  get part for donation to support us. We do not responsible of the contents you used within app");
    description->setWordWrap(true);

    lo_details->addWidget(label_appname);
    lo_details->addWidget(label_version);
    lo_details->addStretch();
    lo_details->addWidget(description);

    auto widget_description = new QWidget(this);
    widget_description->setLayout(lo_details);

    //license
    auto license_txt = new QTextEdit(this);

    QFile file(":/usr/share/doc/elroke/copyright");
    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream readfile(&file);

    QFont font_license;
    font_license.setPointSize(12);

    license_txt->setText(readfile.readAll());
    license_txt->setFont(font_license);
    license_txt->setReadOnly(1);

    //Credit
    auto credit_text = new QTextEdit(this);
    credit_text->setText(tr("Thank to")+" :\n"
                         "Qt\n"
                         "VLC\n"
                         "youtube-dl\n"
                         "All Open Source Community\n\n"
                         "Contributor :\n"
                         "Muhammad Mukharom(mumulala)\n"
                         //keep my name first :D
                         "you");
    credit_text->setReadOnly(1);

    auto  donatur = new QTextEdit (this);

    donatur->setText(tr("Thank to all contributor, keep Elroke alive)")+ ":\n\n"
                     "John\n"
                     "Due\n");
    donatur->setReadOnly(1);

    stack = new QStackedLayout;
    stack->addWidget(widget_description);
    stack->addWidget(license_txt);
    stack->addWidget(credit_text);
    stack->addWidget(donatur);

    connect(tab,&QTabBar::currentChanged,stack,&QStackedLayout::setCurrentIndex);

    auto button_close = new QPushButton(tr("Close"), this);
    connect(button_close,&QPushButton::pressed,this,&about::close);

    auto support =new QPushButton(this);
    support->setIcon(QIcon(":/usr/share/elroke/icon/paypal_donate.png"));
    support->setIconSize(QSize(200,100));
    connect(support,&QPushButton::pressed,[this]()
    {
        QDesktopServices::openUrl(QUrl("https://paypal.me/mukharom"));
    });
    auto layout_button = new QHBoxLayout;
    layout_button->addWidget(button_close,0, Qt::AlignLeft | Qt::AlignBottom);
    layout_button->addWidget(support,0,Qt::AlignRight);

    lo_main->addWidget(tab,0,Qt::AlignCenter);
    lo_main->addLayout(stack);
    lo_main->addLayout(layout_button);

    setLayout(lo_main);

    setMinimumSize(800,600);
    setWindowTitle(tr("About")+ " Elroke");
    setAutoFillBackground(1);
    setWindowFlags(Qt::FramelessWindowHint);
    setPalette(palet);
}
