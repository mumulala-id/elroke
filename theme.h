#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QSettings>
#include <QDebug>
class Theme
{
public:
    Theme(){
           QSettings setting("elroke","elroke");
        setting.beginGroup("Preferences");
        QVariantList colors = setting.value("theme").toList();
//        qDebug()<<colors<<"theme"<<setting.value("menu").toStringList();;
        setting.endGroup();

        if(colors.isEmpty()){
            //set default
            _primaryColor = QColor("#00796B");
            _seCondaryColor = QColor("#009688");
           _backgroundColor = Qt::white;
           _textColor = QColor(255,255,255,128);
           _highlightTextColor = QColor("#E91E63");
        } else {
            _primaryColor =qvariant_cast<QColor>(colors.at(0));
            _seCondaryColor =qvariant_cast<QColor>(colors.at(1));
            _textColor = qvariant_cast<QColor>(colors.at(2));
            _highlightTextColor =qvariant_cast<QColor>( colors.at(3));
            _backgroundColor =qvariant_cast<QColor>(colors.at(4));
        }

    }
    void setPrimaryColor(QColor pColor){
        _primaryColor = pColor;
    }
    void setSecondaryColor(QColor sColor){
    _seCondaryColor = sColor;
}
    void setTextColor(QColor tColor){
        _textColor = tColor;
    }
    void setHighlightTextColor(QColor htColor){
        _highlightTextColor = htColor;
    }
    void setBackgroundColor(QColor bColor){
        _backgroundColor = bColor;
    }
    QColor primaryColor(){
        return _primaryColor;
    }
    QColor secondaryColor(){
        return _seCondaryColor;
    }

    QColor textColor(){
        return _textColor;
    }

    QColor highlightColor(){
        return _highlightTextColor;
    }
    QColor backgroundColor(){
        return _backgroundColor;
    }

    QStringList toStringList(){
        QStringList list =QStringList()<<_primaryColor.name()<<_seCondaryColor.name()<<_textColor.name()<<_highlightTextColor.name()<<_backgroundColor.name();
        return list;
    }
    QVariantList toList(){
        return QVariantList()<<_primaryColor<<_seCondaryColor<<_textColor<<_highlightTextColor<<_backgroundColor;

    }

private:
    QColor _primaryColor;
    QColor _seCondaryColor;
    QColor _textColor;
    QColor _highlightTextColor;
    QColor _backgroundColor;

};

#endif // THEME_H
