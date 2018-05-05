#ifndef THEME_H
#define THEME_H

#include <QColor>

#include <QSettings>
#include <QDebug>
class Theme
{
public:
    Theme();
    void setPrimaryColor(QColor pColor);
    void setSecondaryColor(QColor sColor);
    void setTextColor(QColor tColor);
    void setHighlightTextColor(QColor htColor);
    void setBackgroundColor(QColor bColor);
    QColor primaryColor();
    QColor secondaryColor();
    QColor textColor();
    QColor highlightColor();
    QColor backgroundColor();
    QVariantList toList();
    void setDark();
    void setLight();
    void setButtonColor(QColor btnColor);
    void setButtonTextColor(QColor btColor);
    QColor buttonColor();
    QColor buttonTextColor();
    void setBackgroundTransprency(int transparency);
    int backgroundTransparency();
<<<<<<< HEAD
    void setBasic(Theme t);
=======
>>>>>>> 0fcf4733f1b81e83c79e07dd2487ea1cff017349
private:
    QColor _primaryColor;
    QColor _seCondaryColor;
    QColor _textColor;
    QColor _highlightTextColor;
    QColor _backgroundColor;
    QColor _buttonColor;
    QColor _buttonTextColor;
<<<<<<< HEAD

};
=======
    int alpha=255;


>>>>>>> 0fcf4733f1b81e83c79e07dd2487ea1cff017349
#endif // THEME_H
