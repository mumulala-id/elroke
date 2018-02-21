#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>

class CLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CLineEdit(QWidget *parent =0 );
    ~CLineEdit();

signals :
    void focussed(bool hasFocus);

protected :
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);

};

#endif // CLINEEDIT_H
