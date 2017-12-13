#ifndef OPENING_H
#define OPENING_H
#include <QDialog>
#include <QDesktopWidget>
#include <QApplication>

class opening : public QDialog
{
    Q_OBJECT

private :
    QString title;
    QString singer;
    QDesktopWidget *desktop = QApplication::desktop();
    int desktop_width = desktop->width();
    int desktop_height = desktop->height();


public:
    opening(QString t, QString s, QWidget *parent=0);

};

#endif // OPENING_H
