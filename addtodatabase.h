#ifndef ADDTODATABASE_H
#define ADDTODATABASE_H
#include <QDialog>
#include <QCheckBox>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QListWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QComboBox>
#include <QStandardPaths>
#include <QStringListModel>
#include <QLabel>
#include "youtube_downloader.h"
#include <QDragMoveEvent>
#include <QRadioButton>
#include <theme.h>
class myListWidget :  public QListWidget
{
    Q_OBJECT
public :
    myListWidget(QWidget *parent=nullptr) :
        QListWidget(parent){}

signals :
    void dropped();
protected:
   void dragMoveEvent(QDragMoveEvent *e)
   {
       e->accept();
       }
   void dropEvent(QDropEvent *e)
   {
       QListWidget::dropEvent(e);
       emit dropped();
       e->accept();
   }
};


class addtodatabase : public QDialog
{
    Q_OBJECT

public:
    explicit addtodatabase(QWidget *parent=0);

private :
    QString currentDrive;
    QString currentDirectory;
    QString data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QFileSystemModel *dir_model;
    QTreeView *treeview;
    QFileSystemModel *file_model;
    QLabel *label_currentDirectory;
    QTableView *view;
    QStandardItemModel *model;
    QLineEdit *le_singer;
    QLineEdit *le_language;
    QLineEdit *le_genre;
    QLabel *label_pattern;
    QPushButton *button_start;
    QLineEdit *le_splitter;
    QString splitter{"#"};
    QStringList supported_video {"*.mp4","*.avi","*.dat","*.mkv","*.mpg","*.mov"};
    QComboBox *combo_drive;
    QString default_audio;
    myListWidget *view_pattern;
    Theme theme;

     bool title_first=true;
     bool singer_first=false;
     bool automatic=true;
     QDirIterator::IteratorFlag subdir_flag = QDirIterator::NoIteratorFlags;


private slots:
     void saveToDatabase();
     void getDrive();
     void writeTextStream(const QString &file, QSet<QString> set);
     void pattern();
     QString getSplitter(const QString &);
     QString getCurrentDrive() const { return currentDrive; }

     QDirIterator::IteratorFlag getSubDirFlag() const { return subdir_flag; }
     void getItem();


};

#endif // ADDTODATABASE_H
