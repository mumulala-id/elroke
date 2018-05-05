#ifndef SPINNERPROGRESS_H
#define SPINNERPROGRESS_H

#include <QWidget>


class spinnerProgress : public QWidget
{
    Q_OBJECT
public:
    explicit spinnerProgress(QWidget *parent = nullptr);


private:
    float value=0;
    bool play=false;
    QColor lineColor;
public slots:
    void setValue(uint val);
    void start();
    void stop();
    void setLineColor(QColor);


protected :
    void paintEvent(QPaintEvent *);

};


#endif // SPINNERPROGRESS_H
