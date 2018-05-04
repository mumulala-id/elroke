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
#include <QPair>
#include <QListWidget>

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
    Q_ENUMS(Language)
public:
    explicit preferences(QWidget *parent = nullptr);
    ~preferences();
//    enum Language{English=0,Indonesian=1};
private :
    QString selected_font,selected_background;
    unsigned short  int font_size;
    QCheckBox *check_startapp;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QStringList bg_list ;
    QStringList shortcut_item;
    QThread *thread_resizer;
    QStandardItemModel *model ;
    resizer *img_resizer;
    bool startup=false;
//    QString language;
    unsigned int short language_index;
    QListWidget *list_menu_selected;
    QListWidget *list_theme;
    uint newEntriesLimit;
    QSpinBox *spin_limit_month_newEntries ;
    int themeMode=0;
    QVariantList themeColors;
    int alpha;
//    Language lang;
    QList<QPair<QString, QVariant>> list_change;

};
private slots:
    QStringList getLanguageGenre();
    void handleImage(QImage);
    void apply();
    void ok();
    void readSetting();
    void insertToList();

};

#endif // PREFERENCES_H
