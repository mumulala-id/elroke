#include "clineedit.h"
#include <QPainter>
#include <QDebug>
CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

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

void CLineEdit::mousePressEvent(QMouseEvent *e)
{
    QPainter painter(this);
    if(e->button()==Qt::LeftButton)
        qDebug()<<"tes";
    clear();
    painter.drawText(rect(),"");
}

void CLineEdit::paintEvent(QPaintEvent *){

    QPainter painter(this);
    QPen pen(Qt::white);
    painter.setPen(pen);
    painter.fillRect(rect(), Qt::transparent);
    painter.drawLine(rect().bottomLeft(),rect().bottomRight());

    if(text().isEmpty())
        painter.drawText(rect(), placeholderText());
    painter.drawText(rect(),text());

}
