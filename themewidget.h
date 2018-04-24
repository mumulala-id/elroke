#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QWidget>

class themewidget : public QWidget
{
    Q_OBJECT
public:
    explicit themewidget(QWidget *parent = nullptr);

signals:
protected :
    void paintEvent(QPaintEvent *);

public slots:
};

#endif // THEMEWIDGET_H
