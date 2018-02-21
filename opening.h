#ifndef OPENING_H
#define OPENING_H
#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QPropertyAnimation>

class opening : public QWidget
{
    Q_OBJECT

private :

    QDesktopWidget *desktop = QApplication::desktop();
    int desktop_width = desktop->width();
    int desktop_height = desktop->height();
    QLabel *label_title;
    QLabel *label_singer;
    QPropertyAnimation *title_animaton;
    QPropertyAnimation *singer_animation;
private slots:
    void finish();


public:
    opening(QWidget *parent=0);
    void setData(QString title,QString singer);
    void start();

signals:
    void passed();

};

#endif // OPENING_H
