#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>
//#include <QMouseEvent>

class CLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CLineEdit(QWidget *parent =nullptr );
    ~CLineEdit();

signals :
    void focussed(bool hasFocus);

protected :
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);

};

#endif // CLINEEDIT_H
