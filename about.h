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



private slots:
    void showDescription();
    void showLicense();
    void showCredit();
    void showDonatur();
    void donate();

};

#endif // ABOUT_H
