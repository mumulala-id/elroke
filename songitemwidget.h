#ifndef SONGITEMWIDGET_H
#define SONGITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <song.h>
#include <QStyledItemDelegate>
#include <QPainter>
#include "songitemwidget.h"
#include <QDebug>
#include <QListWidget>

class itemDelegate : public QStyledItemDelegate{

    protected:
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const
    {       
         QStyleOptionViewItem itemOption(option);
        if(option.state & QStyle::State_Selected)
        {
            itemOption.palette.setColor(QPalette::Text,QColor("#9C27B0"));
            painter->fillRect(option.rect, QColor("#9C27B0"));
        } else{
            QPen blackPen(QColor("#9C27B0"));
             painter->setPen(blackPen);
             itemOption.palette.setBrush(QPalette::HighlightedText,Qt::white);
//             itemOption.widget->setPalette(QPalette::Highlight,Qt::red);
            painter->fillRect(option.rect, Qt::white);
            painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
            QStyledItemDelegate::paint(painter, option, index);
        }
    }

};


class songitemwidget : public QWidget
{
    Q_OBJECT
public:
    explicit songitemwidget(QWidget *parent = nullptr);

private :
    Song *song_;
    QLabel *label_title, *label_singer;

signals:

public slots:
    void setSong(Song *);
    Song *song();
};

#endif // SONGITEMWIDGET_H
