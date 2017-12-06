#include "keyboard.h"
#include <QCoreApplication>
#include <QGuiApplication>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDebug>

Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent)

{

    QGridLayout *main_layout = new QGridLayout;

    QString text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

    for(int i=0; i<text.size(); i++){
        keyButton[i] = createButton(text.at(i));
        int row = (i/20);
        int column = (i%20);
        main_layout->addWidget(keyButton[i], row, column);
    }


    AeroButton *space = new AeroButton("\u2423", this);
    space->setFocusPolicy(Qt::NoFocus);
    space->setObjectName("space");
    space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(space,SIGNAL(pressed()),this,SLOT(onButtonClicked()));

    auto *del = new AeroButton("\u232B", this);
    del->setObjectName("backspace");
    del->setMaximumSize(QSize(48,48));
    connect(del,SIGNAL(pressed()),this,SLOT(onButtonClicked()));

    auto *clear = new AeroButton("\u239A", this);
    clear->setMaximumSize(QSize(48,48));
    clear->setObjectName("clear");
    connect(clear,SIGNAL(pressed()),this,SLOT(onButtonClicked()));


    main_layout->setHorizontalSpacing(0);
    main_layout->setVerticalSpacing(0);
    main_layout->addWidget(space,1,16,1,2);
    main_layout->addWidget(del,1,18,1,2);
    main_layout->addWidget(clear,1,19,1,2);
    main_layout->setSpacing(0);
    main_layout->setMargin(0);
    main_layout->setSizeConstraint( QLayout::SetFixedSize );

    setLayout(main_layout);
    setAutoFillBackground(true);
    setWindowFlags(Qt::FramelessWindowHint );
    setMinimumSize(960,96);

}


AeroButton *Keyboard::createButton( const QString &text){

    AeroButton *button = new AeroButton(text, this);

    button->setMaximumSize(QSize(48,48));
    QFont font;
    font.setPointSize(24);

    button->setFont(font);
    button->setFocusPolicy(Qt::NoFocus);
    button->setObjectName(text);
    connect(button,SIGNAL(pressed()),this,SLOT(onButtonClicked()));

    return button;
}

void Keyboard::onButtonClicked(){

 QObject*receiver = qobject_cast<QObject*>(QGuiApplication::focusObject());

         if(!receiver)
             return;

            QPushButton *clickedButton =  qobject_cast<QPushButton *>(sender());
            QString objeck_name = clickedButton->objectName();
            QChar text = objeck_name.at(0);
//            qDebug()<<"ukuran"<<clickedButton->text().size()<<text;

            Qt::Key key;
if(objeck_name.size()>1){

        if(objeck_name=="space"){
            key = Qt::Key_Space;
            QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier," " );
            QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

            QCoreApplication::sendEvent(receiver, &pressEvent);
            QCoreApplication::sendEvent(receiver,&releaseEvent);

        }
        if(objeck_name=="backspace"){
            key = Qt::Key_Backspace;
            QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, 0,0 );
            QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

            QCoreApplication::sendEvent(receiver, &pressEvent);
            QCoreApplication::sendEvent(receiver,&releaseEvent);
//            qDebug()<<"Del";
        }
        if(objeck_name=="clear"){
       QLineEdit *le =     qobject_cast<QLineEdit*>(QGuiApplication::focusObject());
            le->clear();
//            qDebug()<<"clear";
//            key = Qt::Key_Clear;
//            QKeyEvent pressEvent = QKeyEvent(QEvent::KeyPress, key, 0,0);//Qt::NoModifier,QKeySequence(key).toString() );
//            QKeyEvent releaseEvent = QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);

//            QCoreApplication::sendEvent(receiver, &pressEvent);
//            QCoreApplication::sendEvent(receiver,&releaseEvent);
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

    QCoreApplication::sendEvent(receiver, &pressEvent);
    QCoreApplication::sendEvent(receiver,&releaseEvent);
}

}

Keyboard::~Keyboard()
{

}
