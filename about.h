#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QStackedLayout>

class about : public QDialog
{
    Q_OBJECT
public:
   explicit  about(QWidget *parent=nullptr);

private :
    QStackedLayout *stack;

};

#endif // ABOUT_H
