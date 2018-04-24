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
#include <QLabel>
#include <themewidget.h>

class resizer : public QObject
{
    Q_OBJECT
public:
 explicit   resizer(QObject *parent=nullptr) :
    QObject(parent){}
private :
     QStringList _filename;
public slots:
     void setInputs(const QStringList &filenames){
         _filename = filenames;
     }

     void start(){
         foreach (const QString &x, _filename) {
             QImage output(x);
             output = output.scaled(QSize(160,90), Qt::KeepAspectRatio, Qt::SmoothTransformation );
             emit finished(output);
         }
     }

signals :
     void error();
     void finished(QImage output);
};

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
    resizer *img_resizer;
    bool startup=false;
    QString language;

private slots:
    QStringList getLanguageGenre();
    void handleImage(QImage);
    void apply();
    void ok();
    void readSetting();

};

#endif // PREFERENCES_H
