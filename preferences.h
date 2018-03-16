#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QStackedLayout>

class preferences : public QDialog
{
    Q_OBJECT
public:
    explicit preferences(QWidget *parent = nullptr);
private :
    QStackedLayout *stack;
    QString selected_font;

signals:

public slots:
};

#endif // PREFERENCES_H
