#include "clineedit.h"

CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

CLineEdit::~CLineEdit(){}


void CLineEdit::focusInEvent(QFocusEvent *e){


    QLineEdit::focusInEvent(e);
    setReadOnly(false);
    emit(focussed(true));
}

void CLineEdit::focusOutEvent(QFocusEvent *e){


    QLineEdit::focusInEvent(e);
    setReadOnly(true);
    emit(focussed(false));
}
