#ifndef ADDTODATABASE_H
#define ADDTODATABASE_H
#include <QDialog>
#include <QCheckBox>
#include <QTreeView>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QComboBox>

class addtodatabase : public QDialog
{
    Q_OBJECT

public:

    explicit addtodatabase(QWidget *parent=0);

private :

    QString dir_;
    QString working_path;
    QFileSystemModel *dir_model;
    QTreeView *tv_folder;
    QFileSystemModel *file_model;
    QListWidget *lw_list;
    QStringList filelist;
    QCheckBox *cmb_titlefirst;
    QCheckBox *cmb_singerfirst;
    QPushButton *button_start;
    QLineEdit *le_splitter;
    QString splitter{"#"};
    QComboBox *combo_mounted;
    QCheckBox *cb_auto;
    QCheckBox *cb_splitby;
    QString default_singer;
    QString defafult_language;
    QString default_category;

     bool title_first=true;
     bool singer_first=false;
     bool automatic=true;
     bool manual=false;

private slots:

     QString pakDir() const {return dir_;}
     void setDrive(const QString &drive);
     void onTreeviewClicked(const QModelIndex &);
     void splitterChange(QString);
     void saveToDatabase();
     void setSingerFirst(bool);
     void setTitleFirst(bool);
     void getDrive();
     bool isAutomatic()const{return automatic;}
     bool isManual()const{return manual;}
     void setToAuto(bool);
     void setToManual(bool);
     QString getSplitter(const QString &);
     void writeTextStream(const QString &file, QList<QString> set);
     void enableStartButton();

protected :

//     void paintEvent(QPaintEvent *);

};

#endif // ADDTODATABASE_H
