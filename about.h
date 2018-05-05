#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QStackedLayout>
#include "theme.h"

class about : public QDialog
{
    Q_OBJECT
public:
   explicit  about(QWidget *parent=nullptr);

private :
    QStackedLayout *stack;
    Theme theme;

};

#endif // ABOUT_H
