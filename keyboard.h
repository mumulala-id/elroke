#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QPushButton>

class Keyboard : public QWidget
{
    Q_OBJECT
public:
    explicit Keyboard(QWidget *parent = nullptr);
    ~Keyboard();
    void showKeyboard(QPoint);

private:
     QPushButton *keyButton[36];
     bool lowerCase=0;

private slots:
    QPushButton *createButton(const QString &);
    void onButtonClicked();
};

#endif // WIDGET_H
