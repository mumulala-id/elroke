#ifndef SONGITEMWIDGET_H
#define SONGITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <song.h>

class songitemwidget : public QWidget
{
    Q_OBJECT
public:
    explicit songitemwidget(QWidget *parent = nullptr);

private :
    Song *song_;
    QLabel *label_title;
    QLabel *label_singer;

signals:

public slots:
    void setSong(Song *);
    Song *song();
};

#endif // SONGITEMWIDGET_H
