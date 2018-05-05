#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QWidget>
#include <theme.h>

class ThemeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);


protected :
    void paintEvent(QPaintEvent *);

public slots:
    Theme theme();
    void setPrimaryColor(QColor pColor);
    void setSecondaryColor(QColor sColor);
    void setTextColor(QColor);
    void setBackgroundColor(QColor);
    void setButtonColor(QColor);
    void setButtontextColor(QColor);
private slots :
    void setTheme(Theme theme);

private :
    Theme _theme;
};

#endif // THEMEWIDGET_H
