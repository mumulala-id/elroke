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
#include <QComboBox>

class managedb : public QDialog
{
    Q_OBJECT
public:
    explicit managedb(QWidget *parent=0);
    ~managedb();

private :
    QListWidget *list_singer,  *list_language, *list_genre,  *list_folder;
    QCheckBox *check_singer,  *check_language,  *check_genre,   *cb_folder;
    QLineEdit  *le_title_fixed,  *le_set_title,  *le_set_language, *le_set_singer,  *le_set_category;
    QTableView *table;
    ProxyModel *proxy_model;
    QSqlTableModel *sql_model;
    QLabel *total_count_label;
    QLabel *selected_count_label;
    QComboBox *combo_audio_channel;
    bool changeSave=false;

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
    void selectedCount();
    void onListWidgetClicked(QListWidgetItem *);
    void updateList();
    void writeTextStream(const QString &file, QList<QString>set);
    void setAudioChannel();
    void dclose();

};

#endif // MANAGEDB_H
