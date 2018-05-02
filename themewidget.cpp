#include "themewidget.h"
#include <QPainter>
ThemeWidget::ThemeWidget(QWidget *parent) : QWidget(parent)
{
    setMaximumSize(48,48);
}

void ThemeWidget::setTheme(Theme theme)
{
    _theme = theme;
}

Theme ThemeWidget::theme()
{
    return _theme;
}

void ThemeWidget::setPrimaryColor(QColor pColor){
    _theme.setPrimaryColor(pColor);
    update();
}

void ThemeWidget::setSecondaryColor(QColor sColor){
    _theme.setSecondaryColor(sColor);
    update();
}

void ThemeWidget::setTextColor(QColor tColor){
    _theme.setTextColor(tColor);
    update();
}
void ThemeWidget::setBackgroundColor(QColor bColor){
    _theme.setBackgroundColor(bColor);
    update();
}

void ThemeWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    painter.setBrush(primaryColor));
    int w = width()/2;
    painter.setBrush(_theme.primaryColor());
    painter.drawRect(0,0,width(),w);
    painter.setBrush(_theme.secondaryColor());
    painter.drawRect(0,w,width(),w);
    QRect textRect(w/2,w/2,w,w);
    painter.setBrush(_theme.backgroundColor());
    painter.drawRect(textRect);
    painter.setPen(_theme.highlightColor());
    painter.drawText(textRect,"T");


//    painter.drawRect(w,w,w,w);
//    painter.setBrush(Qt::green);
//    painter.drawRect(0,w,w,w);
//    painter.setBrush(Qt::yellow);
//    painter.drawRect(w,0,w,w);
//    painter.setBrush(Qt::black);
//    painter.drawRect(w/2,w/2,w,w);



}
