#ifndef MANAGEDB_H
#define MANAGEDB_H
#include <QDialog>
#include <QListWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableView>
#include "proxymodel.h"
#include <QLabel>
#include <QSqlTableModel>

class managedb : public QDialog
{
    Q_OBJECT
public:
    explicit managedb(QWidget *parent=0);

private :
    QListWidget *list_singer;
    QListWidget *list_language;
    QListWidget *list_genre;
    QListWidget *list_folder;
    QCheckBox *check_singer;
    QCheckBox *check_language;
    QCheckBox *check_genre;
    QCheckBox *check_genre_folder;
    QLineEdit *le_singer;
    QLineEdit *le_language;
    QLineEdit *le_genre;
    QLineEdit *le_folder;
    QLineEdit *le_title_fixed;
    QLineEdit *le_set_title;
    QLineEdit *le_set_language;
    QLineEdit *le_set_singer;
    QLineEdit *le_set_category;
    QTableView *table;
    ProxyModel *proxy_model;
    QSqlTableModel *sql_model;
    QLabel *count_item;

private slots:
    void swapItem(int, int);
    void setitem(QString, int);
    void setTitle();
    void setSinger();
    void setLanguage();
    void setCategory();
    void swapTitleSinger();
    void swapSingerLanguage();
    void swapTitleLanguage();
    void swapSingerCategory();
    void swapTitleCategory();
    void swapLanguageCategory();
    void deleteItem();
    void save();

};

#endif // MANAGEDB_H
