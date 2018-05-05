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
<<<<<<< HEAD
#include <QPainter>
=======
>>>>>>> 0fcf4733f1b81e83c79e07dd2487ea1cff017349
#include <QListWidget>

class previewer : public QWidget
{
    Q_OBJECT
public :
    previewer (QWidget *parent=nullptr) : QWidget(parent){
        backgroundColor =Qt::white;
    };
public slots:
    void setColor(QColor color){
        backgroundColor =color;
        update();
    };
private :
    QColor backgroundColor;
protected:
    void paintEvent(QPaintEvent *){
        QPainter painter(this);
        QPixmap pix(":/usr/share/elroke/icon/transparentbg.png");
        painter.drawPixmap(rect(),pix);
        painter.setPen(Qt::gray);
        painter.setBrush(backgroundColor);
        painter.drawRect(rect());

    }
};

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
    unsigned short  int fontSize;
    QCheckBox *check_startapp;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QStringList bg_list ;
    QStringList favGroup;
    QThread *thread_resizer;
    QStandardItemModel *model ;
    resizer *img_resizer;
    bool startup=false;
//    QString language;
    unsigned int short language_index;
    QListWidget *list_menu_selected;
    QListWidget *list_theme;
    uint newEntriesLimit;
<<<<<<< HEAD
    QSpinBox *monthRange ;
    Theme theme;



=======
    QSpinBox *spin_limit_month_newEntries ;
    int themeMode=0;
    QVariantList themeColors;
    int alpha;
//    Language lang;
    QList<QPair<QString, QVariant>> list_change;

};
>>>>>>> 0fcf4733f1b81e83c79e07dd2487ea1cff017349
private slots:
    QStringList getLanguageGenre();
    void handleImage(QImage);
    void apply();
    void ok();
    void readSetting();
    void insertToList();

};

#endif // PREFERENCES_H
