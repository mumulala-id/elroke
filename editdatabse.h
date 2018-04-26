#ifndef MANAGEDB_H
#define MANAGEDB_H

#include "proxymodel.h"
#include "dbmanager.h"
#include "videowidget.h"
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QStandardPaths>
#include <QTableView>
#include <QLabel>
#include <QSqlTableModel>

class managedb : public QDialog
{
    Q_OBJECT
public:
    explicit managedb(QWidget *parent=nullptr);
    ~managedb();

private :
    QListWidget *list_singer,  *list_language, *list_genre,  *list_folder;
    QTableView *table;
    dbmanager *db;
    ProxyModel *proxy_model;
    QSqlTableModel *sql_model;
    QLabel *total_count_label;
    QLabel *selected_count_label;
    QLineEdit *le_search;
    VideoWidget *video;
    QString app_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    bool anyChange = false;

private slots:
    void swapItem(const int, const int);
    void setitem(const QString &,const int);
    void deleteItem();
    void save();
    void onListWidgetClicked(QListWidgetItem *);
    void updateList();
    void writeTextStream(const QString &file, QList<QString>set);
    void jumpTo(QString);
    void undo();
    void renameFile();
signals :
//    void toSearch(QVariantList);


};

#endif // MANAGEDB_H
