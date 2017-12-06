#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QString>
#include <QToolButton>
#include <QPushButton>

#include "aerobutton.h"

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();

private:

         QPushButton *keyButton[36];

private slots:
    AeroButton *createButton(const QString &);
    void onButtonClicked();


};

#endif // WIDGET_H
