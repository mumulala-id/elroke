#include "clineedit.h"
#include <QPainter>
#include <QDebug>
CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255,255,255,120));
    palette.setColor(QPalette::Text,QColor(0,0,0,80));
    setPalette(palette);
}

CLineEdit::~CLineEdit(){

}

void CLineEdit::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    setReadOnly(false);
    emit(focussed(true));
}

void CLineEdit::focusOutEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    setReadOnly(true);
    emit(focussed(false));
}

