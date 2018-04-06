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

class resizer : public QObject
{
    Q_OBJECT
public:
 explicit   resizer(QObject *parent=0) :
    QObject(parent){}
private :
     QStringList _filename;
public slots:
     void setInputs(const QStringList &filenames){
         _filename = filenames;
     }

     void start(){
         QList<QImage> imglist;
         foreach (const QString &x, _filename) {
             QImage output(x);
             output = output.scaled(QSize(160,90), Qt::KeepAspectRatio, Qt::FastTransformation );
             imglist<<output;
         }
        emit finished( imglist );
     }

signals :
     void error();
     void finished(QList<QImage>output);
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
    void handleImage(QList<QImage> imglist);
    void apply();
    void ok();
    void readSetting();

signals:

public slots:
};

#endif // PREFERENCES_H
