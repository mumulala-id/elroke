#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QString>
#include <QPushButton>

#include <QPushButton>

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();
    void showKeyboard(QPoint);


private:

         QPushButton *keyButton[36];
         bool lowerCase=0;

private slots:
    QPushButton *createButton(const QString &);
    void onButtonClicked();
     void hideKeyboard();
    void capsLock(bool c);


};

#endif // WIDGET_H
