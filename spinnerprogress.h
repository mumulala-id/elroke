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
public slots:
    void setValue(uint val);
    void start();
    void stop();


protected :
    void paintEvent(QPaintEvent *);

};


#endif // SPINNERPROGRESS_H
