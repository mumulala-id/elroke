#include "theme.h"

Theme::Theme(){
    QSettings setting("elroke","elroke");
    setting.beginGroup("Preferences");
    QVariantList colors = setting.value("themeColor").toList();
    setting.endGroup();

        _primaryColor =QColor::fromRgba(colors.at(0).toUInt());
        _seCondaryColor =QColor::fromRgba(colors.at(1).toUInt());
        _textColor = QColor::fromRgba(colors.at(2).toUInt());
        _highlightTextColor =QColor::fromRgba(colors.at(3).toUInt());
        _backgroundColor =QColor::fromRgba(colors.at(4).toUInt());
        _buttonColor =QColor::fromRgba(colors.at(5).toUInt());
        _buttonTextColor = QColor::fromRgba(colors.at(6).toUInt());

}
void Theme::setPrimaryColor(QColor pColor){
    _primaryColor = pColor;
}
void Theme::setSecondaryColor(QColor sColor){
_seCondaryColor = sColor;
}
void Theme::setTextColor(QColor tColor){
    _textColor = tColor;
}
void Theme::setHighlightTextColor(QColor htColor){
    _highlightTextColor = htColor;
}
void Theme::setBackgroundColor(QColor bColor){
    _backgroundColor = bColor;
}
QColor Theme::primaryColor(){
    return _primaryColor;
}
QColor Theme::secondaryColor(){
    return _seCondaryColor;
}

QColor Theme::textColor(){
    return _textColor;
}

QColor Theme::highlightColor(){
    return _highlightTextColor;
}
QColor Theme::backgroundColor(){
    return _backgroundColor;
}

QVariantList Theme::toList(){
    return QVariantList()<<_primaryColor.rgba()<<_seCondaryColor.rgba()
                        <<_textColor.rgba()<<_highlightTextColor.rgba()
                       <<_backgroundColor.rgba()<<_buttonColor.rgba()
                      <<_buttonTextColor.rgba();
}

void Theme::setDark(){
    _backgroundColor = QColor("#424242");
    _textColor = QColor("#FFFFFF");

}

void Theme::setLight(){
    _backgroundColor =QColor(255,255,255);
    _textColor =QColor(0,0,0,128);
}
void Theme::setButtonColor(QColor btnColor){
    _buttonColor = btnColor;
}

void Theme::setButtonTextColor(QColor btColor){
    _buttonTextColor =btColor;
}

QColor Theme::buttonColor(){
    return _buttonColor;
}

QColor Theme::buttonTextColor(){
    return _buttonTextColor;
}

void Theme::setBackgroundTransprency(int transparency){
    _backgroundColor.setAlpha(transparency);
}
int Theme::backgroundTransparency(){
    return _backgroundColor.alpha();
}

void Theme::setBasic(Theme t){
    //background not include
    _primaryColor = t.primaryColor();
    _seCondaryColor = t.secondaryColor();
    _textColor = t.textColor();
    _highlightTextColor = t.highlightColor();
    _buttonColor =t.buttonColor();
    _buttonTextColor =t.buttonTextColor();

}

