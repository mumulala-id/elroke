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
#include <QLabel>
#include <QStandardPaths>
#include "youtube_downloader.h"

class addtodatabase : public QDialog
{
    Q_OBJECT

public:
    explicit addtodatabase(QWidget *parent=0);

private :
    QString currentDrive;
    QString current_dir;
    QString data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QFileSystemModel *dir_model;
    QTreeView *treeview;
    QFileSystemModel *file_model;
    QLabel *label_current_dir;
    QTableView *view;
    QStandardItemModel *model;
    QStringList filelist;
    QCheckBox *cmb_titlefirst;
    QCheckBox *cmb_singerfirst;
    QPushButton *button_start;
    QLineEdit *le_splitter;
    QString splitter{"#"};
    QStringList supported_video = QStringList()<<"*.mp4"<<"*.avi"<<"*.dat"<<"*.mkv"<<"*.mpg"<<"*.mov";
    QComboBox *combo_drive;
    QString default_singer;
    QString defafult_language;
    QString default_genre;
    YoutubeDownloader *ydownloader;

     bool title_first=true;
     bool singer_first=false;
     bool automatic=true;
     QDirIterator::IteratorFlag subdir_flag = QDirIterator::NoIteratorFlags;


private slots:
     QString getCurrentDrive()const {return currentDrive;}
     void splitterChange(QString);
     void saveToDatabase();
     void setSingerFirst(bool);
     void setTitleFirst(bool);
     void getDrive();
     QString getSplitter(const QString &);
     void writeTextStream(const QString &file, QList<QString> set);
     void enableStartButton();
     QDirIterator::IteratorFlag getSubDirFlag()const{return subdir_flag;}
     void getItem();


protected :

};

#endif // ADDTODATABASE_H
