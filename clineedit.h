#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>
#include <QMouseEvent>

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
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

};

#endif // CLINEEDIT_H
