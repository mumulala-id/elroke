#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QStackedLayout>
#include <QStandardItem>
#include <QStandardPaths>
#include <QCheckBox>
#include <QSpinBox>

class preferences : public QDialog
{
    Q_OBJECT
public:
    explicit preferences(QWidget *parent = nullptr);
private :
    QStackedLayout *stack;
    QString selected_font,selected_background;
    int font_size;
    QCheckBox *check_startapp;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

private slots:
    QList<QStandardItem*> getItemList();
    QStringList getLanguageGenre();
    void apply();
    void ok();
    void readSetting();

signals:

public slots:
};

#endif // PREFERENCES_H
