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
#include <theme.h>

class itemDelegate : public QStyledItemDelegate{

    protected:
    void setEditorData();

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const
    {
//            FileQueueListItem* itemWidget = reinterpret_cast<FileQueueListItem*>(index.data(Qt::UserRole).value<void*>());
//         Song * a = index.data(Qt::UserRole+1).value<Song*>();
////            QStringList a = index.data(Qt::UserRole+1).toStringList();
//         qDebug()<<a->getTitle();
         Theme theme;
         QStyleOptionViewItem itemOption(option);
        if(option.state & QStyle::State_Selected)
        {
//            itemOption.palette.setColor(QPalette::Text,QColor("#9C27B0"));
            QColor h =theme.secondaryColor();
            h.setAlpha(60);
            painter->fillRect(option.rect, h);
//            qDebug()<<option.text;
        } else{
//            QPen blackPen(theme.textColor());
                   painter->setBrush(Qt::blue);
//            painter->drawRect(option.rect);

//             painter->setPen(blackPen);
//             itemOption.palette.setBrush(QPalette::HighlightedText,Qt::white);
////             itemOption.widget->setPalette(QPalette::Highlight,Qt::red);
            painter->fillRect(option.rect,Qt::transparent);
            painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
//            painter->drawText(option.rect,"TESTTTTT");
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
