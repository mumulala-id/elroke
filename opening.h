#ifndef OPENING_H
#define OPENING_H
#include <QDialog>

class opening : public QDialog
{
    Q_OBJECT

private :
    QString title;
    QString singer;


public:
    opening(QString t, QString s, QWidget *parent=0);

};

#endif // OPENING_H
