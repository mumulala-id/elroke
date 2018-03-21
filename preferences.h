#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QStackedLayout>
#include <QStandardPaths>
#include <QCheckBox>
#include <QSpinBox>
#include <QThread>
#include <QListWidgetItem>
#include <QStandardItemModel>
class preferences : public QDialog
{
    Q_OBJECT
public:
    explicit preferences(QWidget *parent = nullptr);
    ~preferences();
private :
    QStackedLayout *stack;
    QString selected_font,selected_background;
    unsigned short  int font_size;
    QCheckBox *check_startapp;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QStringList bg_list ;
    QThread *thread_resizer;
    QStandardItemModel *model ;

private slots:
//    QList<QStandardItem*> getItemList();
    QStringList getLanguageGenre();
    void handleImage(QList<QImage> imglist);
    void apply();
    void ok();
    void readSetting();

signals:

public slots:
};

#endif // PREFERENCES_H
