#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QStackedLayout>

class about : public QDialog
{
    Q_OBJECT
public:
   explicit  about(QWidget *parent=0);

private :
    QStackedLayout *stack;

};

#endif // ABOUT_H
