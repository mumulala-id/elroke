/*
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
#include "keyboard.h"
#include <clineedit.h>

#include <QApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QDebug>

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent)

{

    auto main_layout = new QGridLayout;

    auto button = [this](const QString &text){
        auto btn = new QPushButton(text, this);
        btn->setMaximumSize(QSize(48,48));

        QFont font=btn->font();
        font.setPointSize(24);

        btn->setFont(font);
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setObjectName(text);
        connect(btn,&QPushButton::pressed,this,&Keyboard::onButtonClicked);

        return btn;

    };

    QString text = "12345FCDBAEIGHJVWXYZ67890QPTNUOMRSLK";
    QPushButton *keyButton[36];

    for(int i=0; i<text.size(); i++){
        keyButton[i] = button(text.at(i));
        int row = (i/20);
        int column = (i%20);
        main_layout->addWidget(keyButton[i], row, column);
    }

    auto space = new QPushButton("\u2423", this);
    space->setFocusPolicy(Qt::NoFocus);
    space->setObjectName("space");
    space->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    connect(space,&QPushButton::pressed,this,&Keyboard::onButtonClicked);

    auto backspace = new QPushButton("\u232B", this);
    backspace->setObjectName("backspace");
    backspace->setFocusPolicy(Qt::NoFocus);
    backspace->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    backspace->setAutoRepeat(true);
    backspace->setAutoRepeatInterval(30);
    connect(backspace,&QPushButton::pressed,this,&Keyboard::onButtonClicked);
    main_layout->setHorizontalSpacing(0);
    main_layout->setVerticalSpacing(0);
    main_layout->addWidget(space,1,16,1,2);
    main_layout->addWidget(backspace,1,18,1,2);
    main_layout->setSpacing(0);
    main_layout->setMargin(0);

    setLayout(main_layout);
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(960,96);
    QPalette palette;
    palette.setColor(QPalette::ButtonText,QColor(0,0,0,150));
    palette.setColor(QPalette::Base,Qt::gray);
    setPalette(palette);
}

void Keyboard::onButtonClicked(){

 QObject*receiver = qobject_cast<QObject*>(QApplication::focusObject());

         if(!receiver)
             return;

    auto clickedButton =  qobject_cast<QPushButton *>(sender());
    QString objeck_name = clickedButton->objectName();
    QChar text = objeck_name.at(0);

    Qt::Key key;
    if(objeck_name.size()>1)
    {
        if(objeck_name=="space")
        {
            key = Qt::Key_Space;
            QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier," ");
            QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

            QApplication::sendEvent(receiver, &pressEvent);
            QApplication::sendEvent(receiver,&releaseEvent);
        }

        if(objeck_name=="backspace")
        {
            key = Qt::Key_Backspace;
            QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, 0,0 );
            QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

            QApplication::sendEvent(receiver, &pressEvent);
            QApplication::sendEvent(receiver,&releaseEvent);
        }
    }

    else{
        switch(text.unicode()){
        case '0':
            key = Qt::Key_0;
            break;
        case '1':
            key = Qt::Key_1;
            break;
        case '2':
            key = Qt::Key_2;
            break;
        case '3':
            key = Qt::Key_3;
            break;
        case '4':
            key = Qt::Key_4;
            break;
        case '5':
            key = Qt::Key_5;
            break;
        case '6':
            key = Qt::Key_6;
            break;
        case '7' :
            key = Qt::Key_7;
            break;
        case '8':
            key = Qt::Key_8;
            break;
        case '9':
            key = Qt::Key_9;
            break;
        case 'A':
            key = Qt::Key_A;
            break;
        case 'B':
            key = Qt::Key_B;
            break;
        case 'C':
            key = Qt::Key_C;
            break;
        case 'D':
            key = Qt::Key_D;
            break;
        case 'E':
            key = Qt::Key_E;
            break;
        case 'F':
            key = Qt::Key_F;
            break;
        case 'G' :
            key = Qt::Key_G;
            break;
        case 'H':
            key = Qt::Key_H;
            break;
        case 'I':
            key = Qt::Key_I;
            break;
        case 'J':
            key = Qt::Key_J;
            break;
        case 'K':
            key = Qt::Key_K;
            break;
        case 'L':
            key = Qt::Key_L;
            break;
        case 'M':
            key = Qt::Key_M;
            break;
        case 'N':
            key = Qt::Key_N;
            break;
        case 'O':
            key = Qt::Key_O;
            break;
        case 'P' :
            key = Qt::Key_P;
            break;
        case 'Q':
            key = Qt::Key_Q;
            break;
        case 'R':
            key = Qt::Key_R;
            break;
        case 'S':
            key = Qt::Key_S;
            break;
        case 'T':
            key = Qt::Key_T;
            break;
        case 'U':
            key = Qt::Key_U;
            break;
        case 'V':
            key = Qt::Key_V;
            break;
        case 'W':
            key = Qt::Key_W;
            break;
        case 'X' :
            key = Qt::Key_X;
            break;
        case 'Y':
            key = Qt::Key_Y;
            break;
        case 'Z':
            key = Qt::Key_Z;
            break;
        }

    QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier,QKeySequence(key).toString());
    QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

    QApplication::sendEvent(receiver, &pressEvent);
    QApplication::sendEvent(receiver,&releaseEvent);

    }
}

void Keyboard::showKeyboard(QPoint p)
{
    move(p);
    show();
}

Keyboard::~Keyboard()
{

}
